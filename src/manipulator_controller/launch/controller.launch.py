import os
from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.substitutions import Command 
from launch.substitutions import LaunchConfiguration
from launch.actions import DeclareLaunchArgument, TimerAction
from launch.conditions import UnlessCondition
 
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue



def generate_launch_description():

    is_sim_arg = DeclareLaunchArgument(
        "is_sim",
        default_value='true'
    )

    is_sim = LaunchConfiguration("is_sim")

    robot_description = ParameterValue(
        Command(
        [
            "xacro ",      # Call xacro to process the xacro file
            os.path.join(get_package_share_directory("manipulator_description"), "urdf", "manipulator.urdf.xacro"),
            " is_sim:=", is_sim
        ]   
        ),
        value_type=str,
    )

    robot_state_publisher_node = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{"robot_description": robot_description}],     
        condition=UnlessCondition(is_sim)
    )

    controller_manager = Node(
        package="controller_manager",
        executable="ros2_control_node",
        name="controller_manager",
        output="screen",
        parameters=[
            {"robot_description": robot_description,
             "use_sim_time": is_sim,
             "update_rate": 10,
             "arm_controller.type": "joint_trajectory_controller/JointTrajectoryController",
             "arm_controller.joints": ["joint_1", "joint_2", "joint_3"],
             "arm_controller.command_interfaces": ["position"],
             "arm_controller.state_interfaces": ["position"],
             "arm_controller.open_loop_control": True,
             "arm_controller.allow_integration_in_goal_trajectories": True,
             "gripper_controller.type": "joint_trajectory_controller/JointTrajectoryController",
             "gripper_controller.joints": ["joint_4", "joint_5"],
             "gripper_controller.command_interfaces": ["position"],
             "gripper_controller.state_interfaces": ["position"],
             "gripper_controller.open_loop_control": True,
             "gripper_controller.allow_integration_in_goal_trajectories": True,
             "joint_state_broadcaster.type": "joint_state_broadcaster/JointStateBroadcaster"}
        ],
        condition=UnlessCondition(is_sim),
    )
    
#-----The spawner is a small utility that tells the controller_manager to load and start a named controller for your robot.

    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        output="screen",
        arguments=[
            "joint_state_broadcaster",
            "--controller-manager",
            "/controller_manager"
        ]
    )

    arm_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        output="screen",
        arguments=[
            "arm_controller",
            "--controller-manager",
            "/controller_manager"
        ]
    )

    gripper_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        output="screen",
        arguments=[
            "gripper_controller",
            "--controller-manager",
            "/controller_manager"
        ]
    )


    # Use TimerAction to delay spawners, ensuring controller_manager is ready
    delayed_joint_state_broadcaster = TimerAction(
        period=2.0,
        actions=[joint_state_broadcaster_spawner]
    )
    
    delayed_arm_controller = TimerAction(
        period=3.0,
        actions=[arm_controller_spawner]
    )
    
    delayed_gripper_controller = TimerAction(
        period=4.0,
        actions=[gripper_controller_spawner]
    )

    return LaunchDescription([
        is_sim_arg,
        robot_state_publisher_node,
        controller_manager,
        delayed_joint_state_broadcaster,
        delayed_arm_controller,
        delayed_gripper_controller
    ])


