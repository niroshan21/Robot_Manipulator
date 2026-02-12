#!/usr/bin/env python3

import rclpy
import numpy as np
from rclpy.node import Node
from rclpy.action import ActionServer
from manipulator_msgs.action import ManipulatorTask
import moveit_commander
import sys

class TaskServer(Node):
    def __init__(self):
        super().__init__('task_server')
        self.get_logger().info('Starting the Server...')
        
        # Initialize moveit_commander
        moveit_commander.roscpp_initialize(sys.argv)
        self.robot = moveit_commander.RobotCommander()
        self.scene = moveit_commander.PlanningSceneInterface()
        
        # Get planning groups
        self.manipulator_arm = moveit_commander.MoveGroupCommander("arm")
        self.manipulator_gripper = moveit_commander.MoveGroupCommander("gripper")
        
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
        
        # Set joint value targets and plan/execute
        self.manipulator_arm.set_joint_value_target(arm_joint_goal)
        self.manipulator_gripper.set_joint_value_target(gripper_joint_goal)

        # Plan and execute arm movement
        arm_plan = self.manipulator_arm.plan()
        arm_success = False
        if isinstance(arm_plan, tuple):
            # ROS2 Humble returns (success, trajectory, planning_time, error_code)
            arm_success = arm_plan[0]
            arm_trajectory = arm_plan[1]
        else:
            # Older interface
            arm_success = arm_plan.joint_trajectory.points != []
            arm_trajectory = arm_plan
        
        # Plan gripper movement
        gripper_plan = self.manipulator_gripper.plan()
        gripper_success = False
        if isinstance(gripper_plan, tuple):
            gripper_success = gripper_plan[0]
            gripper_trajectory = gripper_plan[1]
        else:
            gripper_success = gripper_plan.joint_trajectory.points != []
            gripper_trajectory = gripper_plan

        if arm_success and gripper_success:
            self.get_logger().info('Planning succeeded. Executing...')
            self.manipulator_arm.execute(arm_trajectory, wait=True)
            self.manipulator_gripper.execute(gripper_trajectory, wait=True)
            goal_handle.succeed()
            result = ManipulatorTask.Result()
            result.success = True
            return result
        else:
            self.get_logger().info('Planning failed for the given task number.')
            goal_handle.abort()
            result = ManipulatorTask.Result()
            result.success = False
            return result

    

def main(args=None):
    rclpy.init(args=args)
    task_server = TaskServer()
    try:
        rclpy.spin(task_server)
    except KeyboardInterrupt:
        pass
    finally:
        moveit_commander.roscpp_shutdown()
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
