#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
import json
import os
import time
from datetime import datetime
from moveit.planning import MoveItPy, PlanningComponent
from moveit.core.robot_state import RobotState


class SimpleTrajectoryGenerator(Node):
    """
    Simple trajectory generator - just specify start and end angles!
    
    Usage:
        1. Edit the START_ANGLES and END_ANGLES below
        2. Run: ros2 launch python_scripts simple_trajectory_generator.launch.py
        3. Your trajectory is saved automatically!
    """
    
    def __init__(self):
        super().__init__('simple_trajectory_generator')
        
        # Declare parameters
        self.declare_parameter('save_directory', 'savedTrajectories')
        self.declare_parameter('trajectory_name', 'custom_trajectory')
        
        # Get parameters
        self.save_directory = self.get_parameter('save_directory').value
        self.trajectory_name = self.get_parameter('trajectory_name').value
        
        # Initialize MoveIt
        self.get_logger().info("Initializing MoveItPy...")
        self.moveit = MoveItPy(node_name="simple_trajectory_generator_moveit")
        
        # Create planning components
        self.get_logger().info("Creating planning components")
        self.arm_component = PlanningComponent("arm", self.moveit)
        
        # Create save directory
        script_dir = os.path.dirname(os.path.abspath(__file__))
        workspace_root = script_dir
        for _ in range(6):  # Navigate up to workspace root
            workspace_root = os.path.dirname(workspace_root)
            if os.path.exists(os.path.join(workspace_root, 'src')):
                break
        
        self.workspace_root = os.path.join(workspace_root, self.save_directory)
        os.makedirs(self.workspace_root, exist_ok=True)
        self.get_logger().info(f"Trajectories will be saved to: {self.workspace_root}")
        
        # Wait for MoveIt to be ready
        time.sleep(2)
    
    def plan_trajectory(self, start_angles, end_angles):
        """
        Plan a trajectory from start to end angles
        
        Args:
            start_angles: [j1, j2, j3, j4] - starting joint positions in radians
            end_angles: [j1, j2, j3, j4] - ending joint positions in radians
        
        Returns:
            dict: Trajectory data ready to save, or None if planning failed
        """
        self.get_logger().info("="*60)
        self.get_logger().info("Planning trajectory:")
        self.get_logger().info(f"  Start: {start_angles}")
        self.get_logger().info(f"  End:   {end_angles}")
        self.get_logger().info("="*60)
        
        # Split into arm (first 3 joints) and gripper (4th joint)
        arm_start = start_angles[:3]
        arm_end = end_angles[:3]
        gripper_start = start_angles[3]
        gripper_end = end_angles[3]
        
        # Set start state
        start_state = RobotState(self.moveit.get_robot_model())
        start_state.set_joint_group_positions("arm", arm_start)
        self.arm_component.set_start_state(robot_state=start_state)
        
        # Set goal state
        goal_state = RobotState(self.moveit.get_robot_model())
        goal_state.set_joint_group_positions("arm", arm_end)
        self.arm_component.set_goal_state(robot_state=goal_state)
        
        # Plan!
        self.get_logger().info("Planning with MoveIt...")
        plan_result = self.arm_component.plan()
        
        if not plan_result:
            self.get_logger().error("✗ Planning failed!")
            return None
        
        self.get_logger().info("✓ Planning successful!")
        
        # Extract trajectory
        traj_msg = plan_result.trajectory.get_robot_trajectory_msg()
        joint_traj = traj_msg.joint_trajectory
        
        # Convert to our format with gripper interpolation
        all_points = []
        for point in joint_traj.points:
            segment_time = point.time_from_start.sec + point.time_from_start.nanosec * 1e-9
            total_time = joint_traj.points[-1].time_from_start.sec + \
                        joint_traj.points[-1].time_from_start.nanosec * 1e-9
            
            # Linear interpolation for gripper
            alpha = segment_time / total_time if total_time > 0 else 0
            gripper_pos = gripper_start + alpha * (gripper_end - gripper_start)
            
            # Combine arm (3 joints) + gripper (1 joint)
            point_data = {
                'positions': list(point.positions) + [gripper_pos],
                'velocities': list(point.velocities) + [0.0] if point.velocities else [],
                'accelerations': list(point.accelerations) + [0.0] if point.accelerations else [],
                'time_from_start_sec': segment_time
            }
            all_points.append(point_data)
        
        # Create complete trajectory data
        trajectory_data = {
            'name': self.trajectory_name,
            'timestamp': datetime.now().isoformat(),
            'planning_group': 'full_robot',
            'planner_id': 'OMPL',
            'start_angles': start_angles,
            'end_angles': end_angles,
            'joint_names': ['joint_1', 'joint_2', 'joint_3', 'joint_4'],
            'points': all_points
        }
        
        return trajectory_data
    
    def save_trajectory(self, trajectory_data):
        """Save trajectory to JSON file"""
        filename = os.path.join(self.workspace_root, f"{self.trajectory_name}.json")
        
        with open(filename, 'w') as f:
            json.dump(trajectory_data, f, indent=2)
        
        total_time = trajectory_data['points'][-1]['time_from_start_sec']
        
        self.get_logger().info("\n" + "="*60)
        self.get_logger().info("✓ TRAJECTORY SAVED!")
        self.get_logger().info("="*60)
        self.get_logger().info(f"File:     {filename}")
        self.get_logger().info(f"Name:     {trajectory_data['name']}")
        self.get_logger().info(f"Points:   {len(trajectory_data['points'])}")
        self.get_logger().info(f"Duration: {total_time:.2f} seconds")
        self.get_logger().info(f"Joints:   {trajectory_data['joint_names']}")
        self.get_logger().info("="*60 + "\n")
        
        return filename
    
    def generate(self, start_angles, end_angles):
        """Main method: plan and save trajectory"""
        trajectory_data = self.plan_trajectory(start_angles, end_angles)
        
        if trajectory_data:
            filename = self.save_trajectory(trajectory_data)
            return filename
        else:
            self.get_logger().error("Failed to generate trajectory!")
            return None


def main():
    """
    EDIT YOUR TRAJECTORY HERE!
    ==========================
    
    Specify your start and end angles below.
    All angles are in RADIANS.
    Format: [joint_1, joint_2, joint_3, joint_4 (gripper)]
    
    Typical ranges:
        joint_1 (base):     -π/2 to π/2    (-1.57 to 1.57)
        joint_2 (shoulder): -π/2 to π/2    (-1.57 to 1.57)
        joint_3 (elbow):    -π/2 to π/2    (-1.57 to 1.57)
        joint_4 (gripper):   0 to 0.7      (0 = closed, 0.7 = open)
    """
    
    # ====================================================================
    # YOUR TRAJECTORY CONFIGURATION - EDIT HERE!
    # ====================================================================
    
    # [0.0, 0.0, 0.0, 0.0]
    # [0.0, 1.1, 0.7, 0.0]
    # [-0.8, 0.8, -0.7, 0.0]
    # [1.5, -0.17, -0.78, 0.0]
    # [0.0, 0.0, 0.0, 0.0]



    START_ANGLES = [1.5, -0.17, -0.78, 0.0]        # Starting position
    END_ANGLES = [0.0, 0.0, 0.0, 0.0]         # Ending position
    TRAJECTORY_NAME = "task_8"    # Name for the saved file
    
    # ====================================================================
    
    
    # Initialize ROS2
    rclpy.init()
    
    # Create node
    generator = SimpleTrajectoryGenerator()
    generator.trajectory_name = TRAJECTORY_NAME
    
    # Generate and save
    print("\n" + "="*60)
    print("SIMPLE TRAJECTORY GENERATOR")
    print("="*60)
    print(f"Trajectory Name: {TRAJECTORY_NAME}")
    print(f"Start:  {START_ANGLES}")
    print(f"End:    {END_ANGLES}")
    print("="*60 + "\n")
    
    try:
        filename = generator.generate(START_ANGLES, END_ANGLES)
        
        if filename:
            print("\n" + "🎉 SUCCESS! 🎉")
            print(f"Your trajectory is ready: {filename}")
            print("\nYou can now use it in your robot program!\n")
        else:
            print("\n❌ FAILED - Check the logs above for errors\n")
    
    except KeyboardInterrupt:
        print("\n\nCancelled by user")
    except Exception as e:
        print(f"\n❌ ERROR: {e}\n")
    finally:
        generator.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
