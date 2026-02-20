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
    ROS2 Node for generating smooth multi-waypoint trajectories
    
    Generates a full trajectory through multiple waypoints:
    - Start: [0, 0, 0, 0] (home position)
    - Waypoint 1: [0, 0.524, 0.524, 0] (raise arm, gripper closed)
    - Waypoint 2: [1.5, 0.524, 0.524, 0] (rotate right, gripper closed)
    - Waypoint 3: [-1.5, 0.524, 0.524, 0] (rotate left, gripper closed)
    """
    
    def __init__(self):
        super().__init__('trajectory_generator')
        
        # Declare parameters
        self.declare_parameter('save_directory', 'savedTrajectories')
        self.declare_parameter('planning_group', 'arm')
        
        # Get parameters
        self.save_directory = self.get_parameter('save_directory').value
        self.planning_group = self.get_parameter('planning_group').value
        
        # Initialize MoveIt
        self.get_logger().info("Initializing MoveItPy...")
        self.moveit = MoveItPy(node_name="trajectory_generator_moveit")
        
        # Create planning components for both arm and gripper
        self.get_logger().info("Creating planning components for arm and gripper")
        self.arm_component = PlanningComponent("arm", self.moveit)
        self.gripper_component = PlanningComponent("gripper", self.moveit)
        
        # Create save directory if it doesn't exist
        # Go up from install/python_scripts/lib to workspace root
        script_dir = os.path.dirname(os.path.abspath(__file__))
        # Traverse up to workspace root
        workspace_root = script_dir
        for _ in range(6):  # Go up enough levels to reach workspace
            workspace_root = os.path.dirname(workspace_root)
            if os.path.exists(os.path.join(workspace_root, 'src')):
                break
        
        self.workspace_root = os.path.join(workspace_root, self.save_directory)
        os.makedirs(self.workspace_root, exist_ok=True)
        self.get_logger().info(f"Trajectories will be saved to: {self.workspace_root}")
        
        # Wait for MoveIt to be ready
        time.sleep(2)
    
    def plan_multi_waypoint_trajectory(self, waypoints):
        """Plan a trajectory through multiple waypoints for arm+gripper (all 4 joints)
        
        Args:
            waypoints: List of joint configurations [j1, j2, j3, j4]
        
        Returns:
            Concatenated trajectory data with all waypoints
        """
        self.get_logger().info(f"Planning trajectory through {len(waypoints)} waypoints")
        
        all_points = []
        time_offset = 0.0
        
        for i in range(len(waypoints) - 1):
            start_config = waypoints[i]
            goal_config = waypoints[i + 1]
            
            self.get_logger().info(f"Segment {i+1}: {start_config} -> {goal_config}")
            
            # Split into arm (first 3 joints) and gripper (joint 4)
            arm_start = start_config[:3]
            arm_goal = goal_config[:3]
            gripper_start = [start_config[3], -start_config[3]]  # joint_4 and mimic joint_5
            gripper_goal = [goal_config[3], -goal_config[3]]
            
            # Set start state for arm
            arm_start_state = RobotState(self.moveit.get_robot_model())
            arm_start_state.set_joint_group_positions("arm", arm_start)
            self.arm_component.set_start_state(robot_state=arm_start_state)
            
            # Set goal state for arm
            arm_goal_state = RobotState(self.moveit.get_robot_model())
            arm_goal_state.set_joint_group_positions("arm", arm_goal)
            self.arm_component.set_goal_state(robot_state=arm_goal_state)
            
            # Plan arm segment
            arm_plan = self.arm_component.plan()
            
            if not arm_plan:
                self.get_logger().error(f"Failed to plan segment {i+1}")
                return None
            
            # Extract trajectory points
            traj_msg = arm_plan.trajectory.get_robot_trajectory_msg()
            joint_traj = traj_msg.joint_trajectory
            
            # Add points from this segment (skip first point if not first segment to avoid duplicate)
            start_idx = 1 if i > 0 else 0
            for point in joint_traj.points[start_idx:]:
                # Combine arm joints (3) with gripper joint (1)
                # Interpolate gripper position for this point
                segment_time = point.time_from_start.sec + point.time_from_start.nanosec * 1e-9
                total_segment_time = joint_traj.points[-1].time_from_start.sec + \
                                    joint_traj.points[-1].time_from_start.nanosec * 1e-9
                
                # Linear interpolation for gripper
                alpha = segment_time / total_segment_time if total_segment_time > 0 else 0
                gripper_pos = gripper_start[0] + alpha * (gripper_goal[0] - gripper_start[0])
                
                # Combine all 4 joints
                combined_positions = list(point.positions) + [gripper_pos]
                combined_velocities = list(point.velocities) + [0.0] if point.velocities else []
                combined_accelerations = list(point.accelerations) + [0.0] if point.accelerations else []
                
                point_data = {
                    'positions': combined_positions,
                    'velocities': combined_velocities,
                    'accelerations': combined_accelerations,
                    'time_from_start_sec': time_offset + segment_time
                }
                all_points.append(point_data)
            
            # Update time offset for next segment
            if joint_traj.points:
                last_time = joint_traj.points[-1].time_from_start.sec + \
                           joint_traj.points[-1].time_from_start.nanosec * 1e-9
                time_offset += last_time
            
            self.get_logger().info(f"  ✓ Segment {i+1} completed: {len(joint_traj.points)} waypoints")
        
        return all_points
    
    def plan_multi_waypoint_trajectory_with_stays(self, waypoints, stay_durations):
        """Plan a trajectory through multiple waypoints with pause periods
        
        Args:
            waypoints: List of joint configurations [j1, j2, j3, j4]
            stay_durations: List of pause durations at each waypoint (in seconds)
        
        Returns:
            Concatenated trajectory data with all waypoints and pauses
        """
        self.get_logger().info(f"Planning trajectory through {len(waypoints)} waypoints with pauses")
        
        all_points = []
        time_offset = 0.0
        
        for i in range(len(waypoints) - 1):
            start_config = waypoints[i]
            goal_config = waypoints[i + 1]
            
            self.get_logger().info(f"Segment {i+1}: {start_config} -> {goal_config}")
            
            # Split into arm (first 3 joints) and gripper (joint 4)
            arm_start = start_config[:3]
            arm_goal = goal_config[:3]
            gripper_start = [start_config[3], -start_config[3]]
            gripper_goal = [goal_config[3], -goal_config[3]]
            
            # Set start state for arm
            arm_start_state = RobotState(self.moveit.get_robot_model())
            arm_start_state.set_joint_group_positions("arm", arm_start)
            self.arm_component.set_start_state(robot_state=arm_start_state)
            
            # Set goal state for arm
            arm_goal_state = RobotState(self.moveit.get_robot_model())
            arm_goal_state.set_joint_group_positions("arm", arm_goal)
            self.arm_component.set_goal_state(robot_state=arm_goal_state)
            
            # Plan arm segment
            arm_plan = self.arm_component.plan()
            
            if not arm_plan:
                self.get_logger().error(f"Failed to plan segment {i+1}")
                return None
            
            # Extract trajectory points
            traj_msg = arm_plan.trajectory.get_robot_trajectory_msg()
            joint_traj = traj_msg.joint_trajectory
            
            # Add points from this segment (skip first point if not first segment to avoid duplicate)
            start_idx = 1 if i > 0 else 0
            for point in joint_traj.points[start_idx:]:
                # Combine arm joints (3) with gripper joint (1)
                # Interpolate gripper position for this point
                segment_time = point.time_from_start.sec + point.time_from_start.nanosec * 1e-9
                total_segment_time = joint_traj.points[-1].time_from_start.sec + \
                                    joint_traj.points[-1].time_from_start.nanosec * 1e-9
                
                # Linear interpolation for gripper
                alpha = segment_time / total_segment_time if total_segment_time > 0 else 0
                gripper_pos = gripper_start[0] + alpha * (gripper_goal[0] - gripper_start[0])
                
                # Combine all 4 joints
                combined_positions = list(point.positions) + [gripper_pos]
                combined_velocities = list(point.velocities) + [0.0] if point.velocities else []
                combined_accelerations = list(point.accelerations) + [0.0] if point.accelerations else []
                
                point_data = {
                    'positions': combined_positions,
                    'velocities': combined_velocities,
                    'accelerations': combined_accelerations,
                    'time_from_start_sec': time_offset + segment_time
                }
                all_points.append(point_data)
            
            # Update time offset for next segment
            if joint_traj.points:
                last_time = joint_traj.points[-1].time_from_start.sec + \
                           joint_traj.points[-1].time_from_start.nanosec * 1e-9
                time_offset += last_time
            
            self.get_logger().info(f"  ✓ Segment {i+1} completed: {len(joint_traj.points)} waypoints")
            
            # Add stay/hold period at this waypoint (if required)
            stay_duration = stay_durations[i + 1]  # Stay duration for the goal of this segment
            if stay_duration > 0:
                self.get_logger().info(f"  Adding {stay_duration}s hold at waypoint {i+1}")
                
                # Create a hold point: same position, but advanced in time
                last_point = all_points[-1].copy()
                last_point['time_from_start_sec'] = time_offset + stay_duration
                last_point['velocities'] = [0.0] * len(last_point['velocities']) if last_point.get('velocities') else []
                last_point['accelerations'] = [0.0] * len(last_point['accelerations']) if last_point.get('accelerations') else []
                
                all_points.append(last_point)
                time_offset += stay_duration
        
        return all_points
    
    def generate_and_save_trajectories(self):
        """Main function to generate smooth multi-waypoint trajectory"""
        self.get_logger().info("="*60)
        self.get_logger().info("Starting Multi-Waypoint Trajectory Generation")
        
        # Define full trajectory waypoints (all 4 joints: j1, j2, j3, j4)
        # Format: [joint_1 (base), joint_2 (shoulder), joint_3 (elbow), joint_4 (gripper)]
        waypoints = [
            [0.0, 0.0, 0.0, 0.0],           # Start: home position (all joints at zero)
            [0.0, 0.524, 0.524, 0.0],       # Waypoint 1: raise arm (shoulder and elbow ~30°), gripper closed
            [1.5, 0.524, 0.524, 0.0],       # Waypoint 2: rotate base right (~86°), arm raised, gripper closed
            [-1.5, 0.524, 0.524, 0.0]       # Waypoint 3: rotate base left (~86°), arm raised, gripper closed
        ]
        
        # Stay durations at each waypoint (in seconds)
        # Index corresponds to waypoint: stay_durations[0] = stay at waypoint 0, etc.
        stay_durations = [0.0, 0.0, 0.0, 0.0]  # No stays - continuous motion through all waypoints
        
        self.get_logger().info("Trajectory waypoints:")
        for i, wp in enumerate(waypoints):
            stay_msg = f" (stay {stay_durations[i]}s)" if stay_durations[i] > 0 else ""
            self.get_logger().info(f"  Waypoint {i}: {wp}{stay_msg}")
        
        # Plan trajectory through all waypoints with stay periods
        trajectory_points = self.plan_multi_waypoint_trajectory_with_stays(waypoints, stay_durations)
        
        if trajectory_points:
            # Create trajectory data structure
            trajectory_data = {
                'name': 'full_smooth_trajectory',
                'timestamp': datetime.now().isoformat(),
                'planning_group': 'full_robot',
                'planner_id': 'OMPL',
                'joint_names': ['joint_1', 'joint_2', 'joint_3', 'joint_4'],
                'points': trajectory_points
            }
            
            # Save to file
            filename = os.path.join(self.workspace_root, "full_smooth_trajectory.json")
            with open(filename, 'w') as f:
                json.dump(trajectory_data, f, indent=2)
            
            total_time = trajectory_points[-1]['time_from_start_sec']
            self.get_logger().info("\n" + "="*60)
            self.get_logger().info("✓ Trajectory Generation Complete!")
            self.get_logger().info("="*60)
            self.get_logger().info(f"Saved trajectory to: {filename}")
            self.get_logger().info(f"Total waypoints: {len(trajectory_points)}")
            self.get_logger().info(f"Total duration: {total_time:.2f} seconds")
            self.get_logger().info(f"Joints: {trajectory_data['joint_names']}")
            self.get_logger().info("="*60)
        else:
            self.get_logger().error("✗ Trajectory generation failed!")


def main(args=None):
    rclpy.init(args=args)
    
    try:
        # Create node
        trajectory_generator = TrajectoryGenerator()
        
        # Wait a bit for everything to be ready
        time.sleep(2)
        
        # Generate and save trajectories
        trajectory_generator.generate_and_save_trajectories()
        
        trajectory_generator.get_logger().info("Shutting down trajectory generator...")
        
        # Use os._exit() to bypass C++ cleanup which causes segfault
        # All trajectories are already saved to disk, so this is safe
        # Exit immediately to avoid any background threads triggering cleanup
        os._exit(0)
        
    except Exception as e:
        get_logger('trajectory_generator').error(f"Error: {str(e)}")
        import traceback
        traceback.print_exc()
        os._exit(1)
    finally:
        # This won't be reached due to os._exit() above
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == '__main__':
    main()
