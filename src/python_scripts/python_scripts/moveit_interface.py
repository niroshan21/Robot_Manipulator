import rclpy
from rclpy.logging import get_logger
import numpy as np
import time
from moveit.planning import MoveItPy, PlanningComponent
from moveit.core.robot_state import RobotState


def move_arm():
    # MoveItPy will get configuration from parameters set by launch file
    manipulator = MoveItPy(node_name="moveit_interface_node")
    #MoveItPy does NOT know your URDF by itself.
    #It reads it from the ROS 2 parameter server, which was populated by your launch file.
    
    # Wait for controllers to be available
    get_logger("moveit_interface").info("Waiting for controllers to be available...")
    time.sleep(5)  # Give controllers time to start

    # Get planning components for arm and gripper
    arm = PlanningComponent("arm", manipulator)   #Create a planning handle for the SRDF group named arm, using the MoveIt system represented by "manipulator".
    gripper = PlanningComponent("gripper", manipulator)
    #What PlanningComponent actually creates internally
        #Look up the group - find in SRDF, get joints and links
        #Attach the robot model - manipulator.get_robot_model() - So now it knows: Joint limits, Kinematic chain, mimic joints, frames
        #Set up planning pipeline - default is OMPL

    # Create robot states for goals
    arm_state = RobotState(manipulator.get_robot_model()) 
    gripper_state = RobotState(manipulator.get_robot_model())
    # A RobotState is: a full representation of the robot (all joints). It stores: every joint name, joint position, full kinematic, all link transforms.
    #Think of it as : “One possible pose of the entire robot in joint space”
    # manipulator.get_robot_model() : returns the RobotModel loaded from URDF+SRDF
    
    # Set joint positions for goals
    arm_state.set_joint_group_positions("arm", [1.57, 0.0, 0.0])
    gripper_state.set_joint_group_positions("gripper", [0.5])  # Only joint_4, joint_5 mimics
    
    # Set start states
    arm.set_start_state_to_current_state()
    gripper.set_start_state_to_current_state()
    #Use the robot’s actual current joint positions as the starting point for planning. It comes from ROS topics. (e.g., /joint_states)
    
    # Set goal states
    arm.set_goal_state(robot_state=arm_state)
    gripper.set_goal_state(robot_state=gripper_state)
    
    # Plan
    arm_plan = arm.plan()
    
    if arm_plan:
        get_logger("moveit_interface").info("Arm plan successful! Executing...")
        # Execute using the robot state from the plan
        manipulator.execute(arm_plan.trajectory, controllers=[])
    else:
        get_logger("moveit_interface").error("Arm planning failed!")
    
    # Plan gripper
    gripper_plan = gripper.plan()
    
    if gripper_plan:
        get_logger("moveit_interface").info("Gripper plan successful! Executing...")
        manipulator.execute(gripper_plan.trajectory, controllers=[])
    else:
        get_logger("moveit_interface").error("Gripper planning failed!")




def main():
    rclpy.init()
    move_arm()
    rclpy.shutdown()

if __name__ == '__main__':
    main()