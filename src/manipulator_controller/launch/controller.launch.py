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

    controllers_yaml = os.path.join(
        get_package_share_directory("manipulator_controller"),
        "config",
        "manipulator_controllers.yaml"
    )

    robot_description = ParameterValue(
        Command(
            [
                "xacro ",
                os.path.join(
                    get_package_share_directory("manipulator_description"),
                    "urdf",
                    "manipulator.urdf.xacro"
                ),
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

    # FIX 1: Separate robot_description and use_sim_time into their own dicts.
    #         Mixing them with the YAML path in one list caused the per-controller
    #         parameter blocks (joints, command_interfaces) to be silently dropped
    #         in ROS 2 Humble's controller_manager.
    controller_manager = Node(
        package="controller_manager",
        executable="ros2_control_node",
        name="controller_manager",
        output="screen",
        parameters=[
            {"robot_description": robot_description},
            {"use_sim_time": False},
            controllers_yaml,
        ],
        condition=UnlessCondition(is_sim),
    )

    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        output="screen",
        arguments=[
            "joint_state_broadcaster",
            "--controller-manager",
            "/controller_manager",
            "--controller-manager-timeout", "30",
        ]
    )

    # FIX 2: Pass --param-file directly to the spawner so that the controller's
    #         joints / command_interfaces parameters are pushed at activation time,
    #         bypassing any parameter-loading-order issues in the controller_manager.
    arm_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        output="screen",
        arguments=[
            "arm_controller",
            "--controller-manager",
            "/controller_manager",
            "--controller-manager-timeout", "30",
            "--param-file", controllers_yaml,
        ]
    )

    gripper_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        output="screen",
        arguments=[
            "gripper_controller",
            "--controller-manager",
            "/controller_manager",
            "--controller-manager-timeout", "30",
            "--param-file", controllers_yaml,
        ]
    )

    # FIX 3: Stagger the spawner delays so they are sequential, not simultaneous.
    #         On a Raspberry Pi the controller_manager starts slowly — give it 5 s.
    #         joint_state_broadcaster must be active before trajectory controllers start.
    return LaunchDescription([
        is_sim_arg,
        robot_state_publisher_node,
        controller_manager,
        TimerAction(period=5.0,  actions=[joint_state_broadcaster_spawner]),
        TimerAction(period=9.0,  actions=[arm_controller_spawner]),
        TimerAction(period=12.0, actions=[gripper_controller_spawner]),
    ])