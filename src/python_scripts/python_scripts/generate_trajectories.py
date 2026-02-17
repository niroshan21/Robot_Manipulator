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
    ROS2 Node for generating and saving the 3 predefined task trajectories
    
    Generates trajectories for:
    - Task 0: Home position [0.0, 0.0, 0.0]
    - Task 1: Pick/place position [-1.14, 1.0, 0.7]
    - Task 2: Alternative position [1.57, 0.5, -0.5]
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
        
    def generate_task_goals(self):
        """Generate the 3 predefined task goals from task_server.py"""
        # These are the exact goals from task_server.py
        # Each task has both arm and gripper goals
        task_goals = [
            {
                'arm': [0.0, 0.0, 0.0],      # Task 0: Home position
                'gripper': [0.7, -0.7]       # Gripper open
            },
            {
                'arm': [-1.14, 1.0, 0.7],    # Task 1: Pick/place position
                'gripper': [0.0, 0.0]        # Gripper closed
            },
            {
                'arm': [1.57, 0.5, -0.5],    # Task 2: Alternative position
                'gripper': [0.5, -0.5]       # Gripper half open
            },
        ]
        
        self.get_logger().info("Task goals loaded:")
        for i, task in enumerate(task_goals):
            self.get_logger().info(f"  Task {i}: arm={task['arm']}, gripper={task['gripper']}")
            
        return task_goals
    
    def plan_to_joint_goal(self, group_name, joint_goal):
        """Plan trajectory to a joint goal for specified group"""
        # Select the appropriate planning component
        planning_component = self.arm_component if group_name == "arm" else self.gripper_component
        
        # Create robot state for goal
        robot_state = RobotState(self.moveit.get_robot_model())
        
    def plan_to_joint_goal(self, group_name, joint_goal):
        """Plan trajectory to a joint goal for specified group"""
        # Select the appropriate planning component
        planning_component = self.arm_component if group_name == "arm" else self.gripper_component
        
        # Create robot state for goal
        robot_state = RobotState(self.moveit.get_robot_model())
        
        # Set joint positions
        robot_state.set_joint_group_positions(group_name, joint_goal)
        
        # Set start state to current
        planning_component.set_start_state_to_current_state()
        
        # Set goal state
        planning_component.set_goal_state(robot_state=robot_state)
        
        # Plan
        self.get_logger().info(f"Planning {group_name} to goal: {joint_goal}")
        plan_result = planning_component.plan()
        
        return plan_result

    
    def save_trajectory(self, plan_result, trajectory_name, group_name):
        """Save trajectory to JSON file"""
        if not plan_result:
            self.get_logger().error("Cannot save - planning failed!")
            return False
        
        robot_model = self.moveit.get_robot_model()
        joint_model_group = robot_model.get_joint_model_group(group_name)
        
        # Initialize trajectory data structure
        trajectory_data = {
            'name': trajectory_name,
            'timestamp': datetime.now().isoformat(),
            'planning_group': group_name,
            'planner_id': 'OMPL',
            'joint_names': list(joint_model_group.active_joint_model_names),
            'points': []
        }
        
        # Try to extract the trajectory
        try:
            # The plan_result.trajectory is a RobotTrajectory object
            # Use get_robot_trajectory_msg() to convert to ROS message format
            traj = plan_result.trajectory
            
            # Convert to ROS message
            traj_msg = traj.get_robot_trajectory_msg()
            
            # Now we can access the joint_trajectory
            joint_traj = traj_msg.joint_trajectory
            
            # Extract each waypoint
            for point in joint_traj.points:
                point_data = {
                    'positions': list(point.positions),
                    'velocities': list(point.velocities) if point.velocities else [],
                    'accelerations': list(point.accelerations) if point.accelerations else [],
                    'time_from_start_sec': point.time_from_start.sec + point.time_from_start.nanosec * 1e-9
                }
                trajectory_data['points'].append(point_data)
            
            # Save to file
            filename = os.path.join(self.workspace_root, f"{trajectory_name}.json")
            with open(filename, 'w') as f:
                json.dump(trajectory_data, f, indent=2)
            
            self.get_logger().info(f"✓ Saved trajectory to: {filename}")
            self.get_logger().info(f"  - Waypoints: {len(trajectory_data['points'])}")
            self.get_logger().info(f"  - Joints: {joint_traj.joint_names}")
            return True
                
        except Exception as e:
            self.get_logger().error(f"Failed to extract trajectory: {e}")
            import traceback
            traceback.print_exc()
            return False
    
    def generate_and_save_trajectories(self):
        """Main function to generate multiple trajectories and save them"""
        self.get_logger().info("="*60)
        self.get_logger().info("Starting Task Trajectory Generation")
        tasks = self.generate_task_goals()
        
        successful_trajectories = 0
        failed_trajectories = 0
        
        for i, task in enumerate(tasks):
            self.get_logger().info(f"\n--- Generating Task {i} Trajectories ---")
            
            # Plan arm trajectory
            arm_plan_result = self.plan_to_joint_goal("arm", task['arm'])
            
            # Plan gripper trajectory
            gripper_plan_result = self.plan_to_joint_goal("gripper", task['gripper'])
            
            if arm_plan_result and gripper_plan_result:
                # Save arm trajectory
                arm_trajectory_name = f"task_{i}_arm_trajectory"
                if self.save_trajectory(arm_plan_result, arm_trajectory_name, "arm"):
                    self.get_logger().info(f"✓ Task {i} arm trajectory saved")
                else:
                    failed_trajectories += 1
                    self.get_logger().error(f"✗ Failed to save Task {i} arm trajectory")
                
                # Save gripper trajectory
                gripper_trajectory_name = f"task_{i}_gripper_trajectory"
                if self.save_trajectory(gripper_plan_result, gripper_trajectory_name, "gripper"):
                    self.get_logger().info(f"✓ Task {i} gripper trajectory saved")
                    successful_trajectories += 1
                else:
                    failed_trajectories += 1
                    self.get_logger().error(f"✗ Failed to save Task {i} gripper trajectory")
            else:
                failed_trajectories += 1
                if not arm_plan_result:
                    self.get_logger().error(f"✗ Arm planning failed for Task {i}")
                if not gripper_plan_result:
                    self.get_logger().error(f"✗ Gripper planning failed for Task {i}")
            
            # Small delay between trajectories
            time.sleep(0.5)
        
        # Summary
        self.get_logger().info("\n" + "="*60)
        self.get_logger().info("Task Trajectory Generation Complete")
        self.get_logger().info("="*60)
        self.get_logger().info(f"Successful: {successful_trajectories}/{len(tasks)}")
        self.get_logger().info(f"Failed: {failed_trajectories}/{len(tasks)}")
        self.get_logger().info(f"Save directory: {self.workspace_root}")
        self.get_logger().info("="*60)


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
