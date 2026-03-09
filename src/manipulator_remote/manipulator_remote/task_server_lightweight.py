#!/usr/bin/env python3

"""
Lightweight Task Server - Uses Pre-generated Trajectories
This version doesn't use MoveIt, making it suitable for resource-constrained hardware like Raspberry Pi.
It loads pre-generated trajectory files and executes them directly.
"""

import rclpy
import json
import os
from rclpy.node import Node
from rclpy.action import ActionServer, GoalResponse
from manipulator_msgs.action import ManipulatorTask
from trajectory_msgs.msg import JointTrajectory, JointTrajectoryPoint
from control_msgs.action import FollowJointTrajectory
from rclpy.action import ActionClient
from builtin_interfaces.msg import Duration


class LightweightTaskServer(Node):
    def __init__(self):
        super().__init__('task_server_lightweight')
        self.get_logger().info('Starting Lightweight Task Server (No MoveIt)...')
        
        # Declare parameters (use_sim_time is already declared by ROS2)
        self.declare_parameter('trajectory_dir', 'savedTrajectories')
        
        # Get trajectory directory
        trajectory_dir = self.get_parameter('trajectory_dir').value
        
        # Find workspace root and trajectory directory
        script_dir = os.path.dirname(os.path.abspath(__file__))
        workspace_root = script_dir
        for _ in range(6):  # Go up enough levels to reach workspace
            workspace_root = os.path.dirname(workspace_root)
            if os.path.exists(os.path.join(workspace_root, 'src')):
                break
        
        self.trajectory_path = os.path.join(workspace_root, trajectory_dir)
        self.get_logger().info(f'Trajectory directory: {self.trajectory_path}')
        
        # Load all trajectories at startup
        self.trajectories = {}
        self.load_all_trajectories()
        
        # Create action clients for arm and gripper controllers
        self.arm_action_client = ActionClient(
            self, 
            FollowJointTrajectory, 
            '/arm_controller/follow_joint_trajectory'
        )
        
        self.gripper_action_client = ActionClient(
            self, 
            FollowJointTrajectory, 
            '/gripper_controller/follow_joint_trajectory'
        )
        
        # Wait for action servers
        self.get_logger().info('Waiting for arm controller...')
        self.arm_action_client.wait_for_server()
        self.get_logger().info('Waiting for gripper controller...')
        self.gripper_action_client.wait_for_server()
        
        # Create action server for task execution
        self.action_server = ActionServer(
            self,
            ManipulatorTask,
            'task_server',
            execute_callback=self.execute_callback,
            goal_callback=self.goal_callback
        )
        
        self.get_logger().info('Lightweight Task Server ready!')
        self.get_logger().info(f'Available tasks: {list(self.trajectories.keys())}')

    def load_all_trajectories(self):
        """Load all pre-generated trajectory files"""
        try:
            # Load tasks 0, 1, 2 (separate arm and gripper files)
            for task_num in range(3):  # Tasks 0, 1, 2
                arm_file = os.path.join(self.trajectory_path, f'task_{task_num}_arm_trajectory.json')
                gripper_file = os.path.join(self.trajectory_path, f'task_{task_num}_gripper_trajectory.json')
                
                if os.path.exists(arm_file) and os.path.exists(gripper_file):
                    with open(arm_file, 'r') as f:
                        arm_data = json.load(f)
                    with open(gripper_file, 'r') as f:
                        gripper_data = json.load(f)
                    
                    self.trajectories[task_num] = {
                        'arm': arm_data,
                        'gripper': gripper_data
                    }
                    
                    self.get_logger().info(
                        f'✓ Loaded Task {task_num}: '
                        f'arm ({len(arm_data["points"])} pts), '
                        f'gripper ({len(gripper_data["points"])} pts)'
                    )
                else:
                    self.get_logger().warn(f'Missing trajectory files for task {task_num}')
            
            # Load task 3 (full smooth trajectory - all 4 joints in one file)
            full_traj_file = os.path.join(self.trajectory_path, 'full_smooth_trajectory.json')
            if os.path.exists(full_traj_file):
                with open(full_traj_file, 'r') as f:
                    full_data = json.load(f)
                
                # Split the 4-joint trajectory into arm (joints 1-3) and gripper (joint 4)
                arm_data = {
                    'name': 'task_3_arm_trajectory',
                    'timestamp': full_data['timestamp'],
                    'planning_group': 'arm',
                    'planner_id': full_data['planner_id'],
                    'joint_names': ['joint_1', 'joint_2', 'joint_3'],
                    'points': []
                }
                
                gripper_data = {
                    'name': 'task_3_gripper_trajectory',
                    'timestamp': full_data['timestamp'],
                    'planning_group': 'gripper',
                    'planner_id': full_data['planner_id'],
                    'joint_names': ['joint_4'],  # Only joint_4, joint_5 is a mimic joint
                    'points': []
                }
                
                # Split each waypoint
                for point in full_data['points']:
                    # Arm gets first 3 joints
                    arm_point = {
                        'positions': point['positions'][:3],
                        'velocities': point['velocities'][:3] if point.get('velocities') else [],
                        'accelerations': point['accelerations'][:3] if point.get('accelerations') else [],
                        'time_from_start_sec': point['time_from_start_sec']
                    }
                    arm_data['points'].append(arm_point)
                    
                    # Gripper gets only 4th joint (joint_5 mimics automatically)
                    gripper_pos = point['positions'][3]
                    gripper_point = {
                        'positions': [gripper_pos],  # Only joint_4
                        'velocities': [point['velocities'][3]] if point.get('velocities') and len(point['velocities']) > 3 else [],
                        'accelerations': [point['accelerations'][3]] if point.get('accelerations') and len(point['accelerations']) > 3 else [],
                        'time_from_start_sec': point['time_from_start_sec']
                    }
                    gripper_data['points'].append(gripper_point)
                
                self.trajectories[3] = {
                    'arm': arm_data,
                    'gripper': gripper_data
                }
                
                self.get_logger().info(
                    f'✓ Loaded Task 3 (Full Smooth Trajectory): '
                    f'arm ({len(arm_data["points"])} pts), '
                    f'gripper ({len(gripper_data["points"])} pts)'
                )
            else:
                self.get_logger().warn(f'Missing full_smooth_trajectory.json for task 3')
                    
        except Exception as e:
            self.get_logger().error(f'Failed to load trajectories: {e}')
            import traceback
            traceback.print_exc()

    def goal_callback(self, goal_request):
        """Accept or reject goals"""
        task_num = goal_request.task_number
        
        if task_num in self.trajectories:
            self.get_logger().info(f'Accepting goal: Task {task_num}')
            return GoalResponse.ACCEPT
        else:
            self.get_logger().warn(
                f'Rejecting goal: Task {task_num} not available. '
                f'Available: {list(self.trajectories.keys())}'
            )
            return GoalResponse.REJECT

    def json_to_joint_trajectory(self, trajectory_data):
        """Convert JSON trajectory data to ROS JointTrajectory message"""
        traj_msg = JointTrajectory()
        traj_msg.joint_names = trajectory_data['joint_names']
        
        for point_data in trajectory_data['points']:
            point = JointTrajectoryPoint()
            point.positions = point_data['positions']
            
            if 'velocities' in point_data and point_data['velocities']:
                point.velocities = point_data['velocities']
            
            if 'accelerations' in point_data and point_data['accelerations']:
                point.accelerations = point_data['accelerations']
            
            # Convert time from float seconds to Duration
            time_sec = point_data['time_from_start_sec']
            point.time_from_start = Duration(
                sec=int(time_sec),
                nanosec=int((time_sec - int(time_sec)) * 1e9)
            )
            
            traj_msg.points.append(point)
        
        return traj_msg

    async def execute_callback(self, goal_handle):
        """Execute the task by sending pre-generated trajectories to controllers"""
        task_num = goal_handle.request.task_number
        
        self.get_logger().info(f'Executing Task {task_num}')
        
        try:
            # Check if task exists
            if task_num not in self.trajectories:
                self.get_logger().error(f'Task {task_num} not found in loaded trajectories!')
                goal_handle.abort()
                result = ManipulatorTask.Result()
                result.success = False
                return result
            
            # Get trajectories for this task
            task_data = self.trajectories[task_num]
            self.get_logger().info(f'Found trajectory data for task {task_num}')
            
            # Convert JSON data to JointTrajectory messages
            arm_trajectory = self.json_to_joint_trajectory(task_data['arm'])
            gripper_trajectory = self.json_to_joint_trajectory(task_data['gripper'])
            
            self.get_logger().info(f'Arm trajectory: {len(arm_trajectory.points)} points')
            self.get_logger().info(f'Gripper trajectory: {len(gripper_trajectory.points)} points')
            
            # Create FollowJointTrajectory goals
            arm_goal = FollowJointTrajectory.Goal()
            arm_goal.trajectory = arm_trajectory
            # Set timestamp to zero to tell controller to start immediately
            arm_goal.trajectory.header.stamp.sec = 0
            arm_goal.trajectory.header.stamp.nanosec = 0
            arm_goal.trajectory.header.frame_id = ''
            
            gripper_goal = FollowJointTrajectory.Goal()
            gripper_goal.trajectory = gripper_trajectory
            # Set timestamp to zero to tell controller to start immediately
            gripper_goal.trajectory.header.stamp.sec = 0
            gripper_goal.trajectory.header.stamp.nanosec = 0
            gripper_goal.trajectory.header.frame_id = ''
            
            # Send arm trajectory
            self.get_logger().info('Sending arm trajectory...')
            arm_future = self.arm_action_client.send_goal_async(arm_goal)
            arm_goal_handle = await arm_future
            
            if not arm_goal_handle.accepted:
                self.get_logger().error('Arm trajectory rejected!')
                goal_handle.abort()
                result = ManipulatorTask.Result()
                result.success = False
                return result
            
            self.get_logger().info('Arm trajectory accepted, waiting for completion...')
            
            # Wait for arm to complete
            arm_result_future = arm_goal_handle.get_result_async()
            arm_result = await arm_result_future
            
            self.get_logger().info(f'Arm trajectory completed with status: {arm_result.status}')
            
            # Send gripper trajectory
            self.get_logger().info('Sending gripper trajectory...')
            gripper_future = self.gripper_action_client.send_goal_async(gripper_goal)
            gripper_goal_handle = await gripper_future
            
            if not gripper_goal_handle.accepted:
                self.get_logger().error('Gripper trajectory rejected!')
                goal_handle.abort()
                result = ManipulatorTask.Result()
                result.success = False
                return result
            
            self.get_logger().info('Gripper trajectory accepted, waiting for completion...')
            
            # Wait for gripper to complete
            gripper_result_future = gripper_goal_handle.get_result_async()
            gripper_result = await gripper_result_future
            
            self.get_logger().info(f'Gripper trajectory completed with status: {gripper_result.status}')
            
            # Success!
            goal_handle.succeed()
            result = ManipulatorTask.Result()
            result.success = True
            self.get_logger().info(f'Task {task_num} completed successfully!')
            return result
            
        except KeyError as e:
            self.get_logger().error(f'Missing trajectory data key: {e}')
            import traceback
            traceback.print_exc()
            goal_handle.abort()
            result = ManipulatorTask.Result()
            result.success = False
            return result
        except Exception as e:
            self.get_logger().error(f'Task execution failed: {e}')
            import traceback
            traceback.print_exc()
            goal_handle.abort()
            result = ManipulatorTask.Result()
            result.success = False
            return result


def main(args=None):
    rclpy.init(args=args)
    task_server = LightweightTaskServer()
    rclpy.spin(task_server)
    task_server.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
