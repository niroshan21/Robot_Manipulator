#!/usr/bin/env python3

import rclpy
import numpy as np
from rclpy.node import Node
from rclpy.action import ActionServer, ActionClient
from manipulator_msgs.action import ManipulatorTask
from moveit_msgs.action import MoveGroup
from moveit_msgs.msg import Constraints, JointConstraint, MotionPlanRequest
from sensor_msgs.msg import JointState
import time

class TaskServer(Node):
    def __init__(self):
        super().__init__('task_server')
        self.get_logger().info('Starting the Server...')
        
        # Create action clients for MoveIt2 MoveGroup
        self.move_group_client = ActionClient(self, MoveGroup, '/move_action')
        
        # Wait for action server
        self.get_logger().info('Waiting for MoveGroup action server...')
        self.move_group_client.wait_for_server()
        
        self.action_server = ActionServer(
            self,  # The node that owns the action server.
            ManipulatorTask,  # The action type - defines goal, result, feedback
            'task_server', # The name of the action
            self.goalCallback,  # The callback function to handle incoming goals
        )

        self.get_logger().info('Action Server has been started.')

    # This function is automatically invoked by ROS 2 when: an action client sends a goal to /task_server.
    def goalCallback(self, goal_handle):  # goal_handle: it contains the goal request data
        self.get_logger().info('Received goal request with task number %d' % goal_handle.request.task_number)

        arm_joint_goal = []
        gripper_joint_goal = []
        arm_joint_names = ['joint_1', 'joint_2', 'joint_3']
        gripper_joint_names = ['joint_4', 'joint_5']

        if goal_handle.request.task_number == 0:
            arm_joint_goal = [0.0, 0.0, 0.0]
            gripper_joint_goal = [0.7, -0.7] 
        elif goal_handle.request.task_number == 1:
            arm_joint_goal = [-1.14, 1.0, 0.7]
            gripper_joint_goal = [0.0, 0.0]
        elif goal_handle.request.task_number == 2:
            arm_joint_goal = [1.57, 0.5, -0.5]
            gripper_joint_goal = [0.5, -0.5]
        else:
            self.get_logger().info('Invalid task number received.')
            goal_handle.abort()
            result = ManipulatorTask.Result()
            result.success = False
            return result
        
        # Execute arm movement
        arm_success = self._move_joints('arm', arm_joint_names, arm_joint_goal)
        
        # Execute gripper movement
        gripper_success = self._move_joints('gripper', gripper_joint_names, gripper_joint_goal)

        if arm_success and gripper_success:
            self.get_logger().info('Execution succeeded.')
            goal_handle.succeed()
            result = ManipulatorTask.Result()
            result.success = True
            return result
        else:
            self.get_logger().info('Execution failed.')
            goal_handle.abort()
            result = ManipulatorTask.Result()
            result.success = False
            return result
    
    def _move_joints(self, group_name, joint_names, joint_positions):
        """Send joint goal to MoveIt2 via action"""
        goal_msg = MoveGroup.Goal()
        
        # Set planning group
        goal_msg.request.group_name = group_name
        
        # Create joint constraints
        goal_msg.request.goal_constraints.append(Constraints())
        for name, position in zip(joint_names, joint_positions):
            joint_constraint = JointConstraint()
            joint_constraint.joint_name = name
            joint_constraint.position = position
            joint_constraint.tolerance_above = 0.01
            joint_constraint.tolerance_below = 0.01
            joint_constraint.weight = 1.0
            goal_msg.request.goal_constraints[0].joint_constraints.append(joint_constraint)
        
        # Set other planning parameters
        goal_msg.request.num_planning_attempts = 10
        goal_msg.request.allowed_planning_time = 5.0
        goal_msg.request.max_velocity_scaling_factor = 0.1
        goal_msg.request.max_acceleration_scaling_factor = 0.1
        goal_msg.planning_options.plan_only = False
        
        # Send goal and wait
        self.get_logger().info(f'Sending goal for {group_name}...')
        send_goal_future = self.move_group_client.send_goal_async(goal_msg)
        rclpy.spin_until_future_complete(self, send_goal_future, timeout_sec=10.0)
        
        goal_handle = send_goal_future.result()
        if not goal_handle.accepted:
            self.get_logger().error(f'{group_name}: Goal rejected')
            return False
        
        # Wait for result
        get_result_future = goal_handle.get_result_async()
        rclpy.spin_until_future_complete(self, get_result_future, timeout_sec=30.0)
        
        result = get_result_future.result()
        if result:
            self.get_logger().info(f'{group_name}: Execution completed')
            return True
        else:
            self.get_logger().error(f'{group_name}: Execution failed')
            return False

    

def main(args=None):
    rclpy.init(args=args)
    task_server = TaskServer()
    try:
        rclpy.spin(task_server)
    except KeyboardInterrupt:
        pass
    finally:
        task_server.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()



# FOR RASPBERRY PI (ROS2 Humble with moveit_commander)
# sudo apt install ros-humble-moveit-commander

# cd ~/Robot_Manipulator
# colcon build

# . install/setup.bash
# ros2 run manipulator_remote task_server_rpi

# In another terminal:
# . install/setup.bash
# ros2 action list
# ros2 action send_goal /task_server manipulator_msgs/action/ManipulatorTask "{task_number: 1}" 
