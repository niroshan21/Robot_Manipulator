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

    # Loaded into the C++ ros2_control_node — contains ONLY controller types
    # and update_rate. Passing per-controller blocks here crashes the node (exit -6).
    controller_manager_yaml = os.path.join(
        get_package_share_directory("manipulator_controller"),
        "config",
        "controller_manager.yaml"
    )

    # Loaded by the spawner via --param-file — contains ONLY per-controller
    # params (joints, command_interfaces, state_interfaces).
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

    # Only launched on real robot (not sim).
    robot_state_publisher_node = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{"robot_description": robot_description}],
        condition=UnlessCondition(is_sim)
    )

    # Pass only controller_manager.yaml here — it only contains controller_manager
    # parameters so the C++ node won't crash on unknown top-level keys.
    controller_manager = Node(
        package="controller_manager",
        executable="ros2_control_node",
        name="controller_manager",
        output="screen",
        parameters=[
            {"robot_description": robot_description},
            {"use_sim_time": False},
            controller_manager_yaml,    # safe — only controller_manager params
        ],
        condition=UnlessCondition(is_sim),
    )

    # joint_state_broadcaster needs no --param-file (no joints/interfaces to declare).
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

    # --param-file pushes arm_controller's joints/command_interfaces to the
    # controller via set_parameters before configure() is called.
    arm_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        output="screen",
        arguments=[
            "arm_controller",
            "--controller-manager", "/controller_manager",
            "--controller-manager-timeout", "30",
            "--param-file", controllers_yaml,
        ]
    )

    # Same for gripper_controller.
    gripper_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        output="screen",
        arguments=[
            "gripper_controller",
            "--controller-manager", "/controller_manager",
            "--controller-manager-timeout", "30",
            "--param-file", controllers_yaml,
        ]
    )

    # Staggered delays for Raspberry Pi:
    #   5s  — controller_manager fully initializes
    #   9s  — joint_state_broadcaster active before arm starts
    #   12s — arm active before gripper starts
    return LaunchDescription([
        is_sim_arg,
        robot_state_publisher_node,
        controller_manager,
        TimerAction(period=5.0,  actions=[joint_state_broadcaster_spawner]),
        TimerAction(period=9.0,  actions=[arm_controller_spawner]),
        TimerAction(period=12.0, actions=[gripper_controller_spawner]),
    ])
