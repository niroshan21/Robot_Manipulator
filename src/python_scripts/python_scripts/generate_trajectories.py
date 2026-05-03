#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from rclpy.logging import get_logger
import json
import os
import sys
from datetime import datetime
import numpy as np
import time
from moveit.planning import MoveItPy, PlanningComponent
from moveit.core.robot_state import RobotState
from geometry_msgs.msg import Pose, Point, Quaternion
from moveit_msgs.msg import RobotTrajectory


class TrajectoryGenerator(Node):
    """
    ROS2 Node for generating smooth multi-waypoint trajectories.

    Generates an oscillating trajectory with 7 waypoints:
      - Start : [0, 0, 0, 0]          home position
      - WP 1  : [0, 0.5, 0.5, 0]     raise arm
      - WP 2-6: oscillate base ±86°/69°
    """

    # ── Constants ──────────────────────────────────────────────────────────────
    MAX_PLAN_ATTEMPTS = 3          # OMPL retry limit  (fix issue 7)
    PLAN_RETRY_DELAY  = 0.5        # seconds between retries

    def __init__(self):
        super().__init__('trajectory_generator')

        # Declare parameters
        self.declare_parameter('save_directory',  'savedTrajectories')
        self.declare_parameter('planning_group',   'arm')           # kept for future use
        self.declare_parameter('speed_scale',      8.0)             # 4x faster by default

        self.save_directory  = self.get_parameter('save_directory').value
        self.planning_group  = self.get_parameter('planning_group').value  # fix issue 10 — stored but now used below
        self.speed_scale     = float(self.get_parameter('speed_scale').value)

        # ── MoveIt init ────────────────────────────────────────────────────────
        self.get_logger().info("Initialising MoveItPy …")
        try:
            self.moveit = MoveItPy(node_name="trajectory_generator_moveit")
        except Exception as exc:                                    # fix issue 8
            self.get_logger().error(f"MoveItPy init failed: {exc}")
            raise

        self.get_logger().info(f"Creating planning components  (arm + gripper)")
        # fix issue 10 — use self.planning_group for the arm component
        self.arm_component     = PlanningComponent(self.planning_group, self.moveit)
        self.gripper_component = PlanningComponent("gripper",           self.moveit)

        # ── Save directory ─────────────────────────────────────────────────────
        script_dir     = os.path.dirname(os.path.abspath(__file__))
        workspace_root = script_dir
        for _ in range(6):
            workspace_root = os.path.dirname(workspace_root)
            if os.path.exists(os.path.join(workspace_root, 'src')):
                break

        self.save_path = os.path.join(workspace_root, self.save_directory)
        os.makedirs(self.save_path, exist_ok=True)
        self.get_logger().info(f"Trajectories will be saved to: {self.save_path}")

        time.sleep(2)   # let MoveIt settle

    # ── Private helpers ────────────────────────────────────────────────────────

    def _plan_arm_segment(self, arm_start, arm_goal):
        """
        Plan one arm segment with retry.
        Returns the JointTrajectory message, or None on failure.
        fix issue 7 — OMPL retry loop.
        """
        arm_start_state = RobotState(self.moveit.get_robot_model())
        arm_start_state.set_joint_group_positions(self.planning_group, arm_start)
        self.arm_component.set_start_state(robot_state=arm_start_state)

        arm_goal_state = RobotState(self.moveit.get_robot_model())
        arm_goal_state.set_joint_group_positions(self.planning_group, arm_goal)
        self.arm_component.set_goal_state(robot_state=arm_goal_state)

        for attempt in range(1, self.MAX_PLAN_ATTEMPTS + 1):
            arm_plan = self.arm_component.plan()
            if arm_plan:
                return arm_plan.trajectory.get_robot_trajectory_msg().joint_trajectory
            self.get_logger().warn(
                f"  Planning attempt {attempt}/{self.MAX_PLAN_ATTEMPTS} failed — retrying …"
            )
            time.sleep(self.PLAN_RETRY_DELAY)

        return None     # all attempts exhausted

    @staticmethod
    def _segment_duration(joint_traj):
        """Return total duration of a JointTrajectory in seconds."""
        if not joint_traj.points:
            return 0.0
        last = joint_traj.points[-1].time_from_start
        return last.sec + last.nanosec * 1e-9

    @staticmethod
    def _point_time(point):
        """Convert a trajectory point's time_from_start to float seconds."""
        return point.time_from_start.sec + point.time_from_start.nanosec * 1e-9

    # ── Main planning method ───────────────────────────────────────────────────

    def plan_trajectory_with_stays(self, waypoints, stay_durations):
        """
        Plan a trajectory through *waypoints* with optional hold periods.

        Args:
            waypoints      : list of [j1, j2, j3, j4] configurations.
            stay_durations : hold time (s) at each waypoint.
                             stay_durations[k] = hold AFTER arriving at waypoints[k].
                             stay_durations[0]  = hold before the first move.
                             (fix issue 3 — clearly documented; index matches waypoint)

        Returns:
            List of point-dicts, or None on planning failure.
        """
        self.get_logger().info(
            f"Planning trajectory through {len(waypoints)} waypoints …"
        )
        if self.speed_scale <= 0.0:
            self.get_logger().warn(
                f"Invalid speed_scale={self.speed_scale}; using 1.0"
            )
            self.speed_scale = 1.0
        time_scale = 1.0 / self.speed_scale

        all_points  = []
        time_offset = 0.0

        # ── Optional hold at the very first waypoint (fix issue 3) ────────────
        if stay_durations[0] > 0:
            self.get_logger().info(
                f"  Adding {stay_durations[0] * time_scale:.2f}s hold at start waypoint"
            )
            # Two identical zero-velocity points spanning the hold (fix issue 9)
            hold_pos = list(waypoints[0])
            n_joints = len(hold_pos)
            all_points.append({
                'positions':          hold_pos,
                'velocities':         [0.0] * n_joints,
                'accelerations':      [0.0] * n_joints,
                'time_from_start_sec': 0.0,
            })
            all_points.append({
                'positions':          hold_pos,
                'velocities':         [0.0] * n_joints,
                'accelerations':      [0.0] * n_joints,
                'time_from_start_sec': stay_durations[0] * time_scale,
            })
            time_offset += stay_durations[0] * time_scale

        # ── Segment loop ───────────────────────────────────────────────────────
        for i in range(len(waypoints) - 1):
            start_cfg = waypoints[i]
            goal_cfg  = waypoints[i + 1]

            self.get_logger().info(
                f"  Segment {i + 1}: {start_cfg} → {goal_cfg}"
            )

            arm_start = start_cfg[:3]
            arm_goal  = goal_cfg[:3]

            gripper_start_pos = start_cfg[3]
            gripper_goal_pos  = goal_cfg[3]

            # Plan arm segment (with retry)
            joint_traj = self._plan_arm_segment(arm_start, arm_goal)
            if joint_traj is None:
                self.get_logger().error(f"  Segment {i + 1} planning failed after all retries.")
                return None

            seg_duration = self._segment_duration(joint_traj)
            scaled_duration = seg_duration * time_scale

            # Skip duplicate first point for all segments after the first
            # fix issue 4 — always skip point[0] after segment 0 to avoid
            # using MoveIt's "current state" snapshot which may differ from waypoints[i]
            # Also skip point[0] on the very first segment if we already added
            # a start hold, to keep time strictly increasing.
            start_idx = 1 if (i > 0 or time_offset > 0.0) else 0

            for point in joint_traj.points[start_idx:]:
                seg_t = self._point_time(point) * time_scale

                # Gripper: linear position interpolation over arm segment time
                alpha       = (seg_t / scaled_duration) if scaled_duration > 0 else 1.0
                gripper_pos = gripper_start_pos + alpha * (gripper_goal_pos - gripper_start_pos)

                # fix issue 5 — gripper velocity computed from position derivative,
                # not hardcoded to 0.  dpos/dt = (goal - start) / duration
                gripper_vel = (
                    (gripper_goal_pos - gripper_start_pos) / scaled_duration
                    if scaled_duration > 0 else 0.0
                )
                # Taper velocity to 0 at segment endpoints for continuity
                taper = 4.0 * alpha * (1.0 - alpha)   # peaks at 1 at midpoint, 0 at ends
                gripper_vel *= taper

                arm_vels  = list(point.velocities)    if point.velocities    else [0.0] * 3
                arm_accels = list(point.accelerations) if point.accelerations else [0.0] * 3

                all_points.append({
                    'positions':           list(point.positions) + [gripper_pos],
                    'velocities':          arm_vels              + [gripper_vel],
                    'accelerations':       arm_accels            + [0.0],
                    'time_from_start_sec': time_offset + seg_t,
                })

            time_offset += scaled_duration
            self.get_logger().info(
                f"  ✓ Segment {i + 1}: {len(joint_traj.points)} arm points, "
                f"duration {scaled_duration:.2f}s"
            )

            # ── Hold at goal of this segment (fix issue 9 — monotonic time) ──
            stay = stay_durations[i + 1]   # hold AFTER reaching waypoints[i+1]
            if stay > 0:
                self.get_logger().info(
                    f"  Adding {stay * time_scale:.2f}s hold at waypoint {i + 1}"
                )
                last = all_points[-1]
                n    = len(last['positions'])

                # Ensure the final segment point is a stop before holding
                last['velocities'] = [0.0] * n
                last['accelerations'] = [0.0] * n

                # Single hold point to keep time strictly increasing
                all_points.append({
                    'positions':           list(last['positions']),
                    'velocities':          [0.0] * n,
                    'accelerations':       [0.0] * n,
                    'time_from_start_sec': time_offset + (stay * time_scale),
                })
                time_offset += stay * time_scale

        return all_points

    # ── Top-level entry point ──────────────────────────────────────────────────

    def generate_and_save_trajectories(self):
        """Generate and save the oscillating multi-waypoint trajectory."""
        self.get_logger().info("=" * 60)
        self.get_logger().info("Starting Multi-Waypoint Trajectory Generation")
        self.get_logger().info("=" * 60)

        # [j1 base, j2 shoulder, j3 elbow, j4 gripper]
        waypoints = [
            [ 0.0,  0.0,  0.0,  0.0],   # 0: home
            [ 0.0,  1.1,  0.0,  0.0],   # 1: raise arm
            [ 1.57,  1.1,  0.0,  0.0],   # 2: rotate right  (+86°)
            [-1.57,  1.1,  0.0,  0.0],   # 3: rotate left   (−69°)
            [ 1.57,  1.1,  0.0,  0.0],   # 4: rotate right  again
            [-1.57,  1.1,  0.0,  0.0],   # 5: rotate left   again
            [ 0.0,  1.1,  0.0,  0.0],   # 6: end at right
            [ 0.0,  1.4,  0.7,  0.0],   # 7: return to home
        ]

        # stay_durations[k] = hold (seconds) AFTER arriving at waypoints[k]
        # Index 0 = hold before any movement.
        stay_durations = [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]

        self.get_logger().info("Waypoints:")
        for k, wp in enumerate(waypoints):
            stay_msg = f"  (hold {stay_durations[k]}s)" if stay_durations[k] > 0 else ""
            self.get_logger().info(f"  [{k}] {wp}{stay_msg}")

        trajectory_points = self.plan_trajectory_with_stays(waypoints, stay_durations)

        if not trajectory_points:
            self.get_logger().error("✗ Trajectory generation failed — nothing saved.")
            return False

        trajectory_data = {
            'name':           'full_smooth_trajectory',
            'timestamp':       datetime.now().isoformat(),
            'planning_group': 'arm + gripper',
            'planner_id':     'OMPL',
            'joint_names':    ['joint_1', 'joint_2', 'joint_3', 'joint_4'],
            'points':          trajectory_points,
        }

        filename = os.path.join(self.save_path, "task_6.json")
        with open(filename, 'w') as f:
            json.dump(trajectory_data, f, indent=2)

        total_time = trajectory_points[-1]['time_from_start_sec']
        self.get_logger().info("\n" + "=" * 60)
        self.get_logger().info("✓ Trajectory Generation Complete!")
        self.get_logger().info("=" * 60)
        self.get_logger().info(f"  Saved to   : {filename}")
        self.get_logger().info(f"  Points     : {len(trajectory_points)}")
        self.get_logger().info(f"  Duration   : {total_time:.2f}s")
        self.get_logger().info(f"  Joints     : {trajectory_data['joint_names']}")
        self.get_logger().info("=" * 60)
        return True


# ── Entry point ────────────────────────────────────────────────────────────────

def main(args=None):
    rclpy.init(args=args)

    try:
        node = TrajectoryGenerator()     # fix issue 8 — __init__ exceptions propagate here
    except Exception as exc:
        get_logger('trajectory_generator').error(
            f"Failed to initialise TrajectoryGenerator: {exc}"
        )
        os._exit(1)

    time.sleep(2)   # let MoveIt finish starting up

    success = False
    try:
        success = node.generate_and_save_trajectories()
    except Exception as exc:
        node.get_logger().error(f"Unhandled error during generation: {exc}")
        import traceback
        traceback.print_exc()

    # os._exit() is intentional — avoids MoveIt/rclpy C++ cleanup segfault
    os._exit(0 if success else 1)


if __name__ == '__main__':
    main()