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

    # joint_state_broadcaster does NOT need --param-file because it has no
    # per-controller parameters (joints/command_interfaces) to load.
    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        output="screen",
        arguments=[
            "joint_state_broadcaster",
            "--controller-manager", "/controller_manager",
            "--controller-manager-timeout", "30",
        ]
    )

    # FIX: --param-file explicitly delivers the arm_controller parameter block
    # (joints, command_interfaces, state_interfaces) to the controller via a
    # set_parameters service call BEFORE configure() is called.
    # This bypasses Humble's unreliable global context mechanism entirely.
    arm_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        output="screen",
        arguments=[
            "arm_controller",
            "--controller-manager", "/controller_manager",
            "--controller-manager-timeout", "30",
            "--param-file", controllers_yaml,   # <-- THE FIX
        ]
    )

    # FIX: Same fix applied to gripper_controller.
    gripper_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        output="screen",
        arguments=[
            "gripper_controller",
            "--controller-manager", "/controller_manager",
            "--controller-manager-timeout", "30",
            "--param-file", controllers_yaml,   # <-- THE FIX
        ]
    )

    return LaunchDescription([
        is_sim_arg,
        robot_state_publisher_node,
        controller_manager,
        TimerAction(period=5.0,  actions=[joint_state_broadcaster_spawner]),
        TimerAction(period=9.0,  actions=[arm_controller_spawner]),
        TimerAction(period=12.0, actions=[gripper_controller_spawner]),
    ])