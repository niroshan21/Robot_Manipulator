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

    # Resolve YAML path once and reuse
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
    # robot_state_publisher publishes the TF tree and /robot_description topic.
    robot_state_publisher_node = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{"robot_description": robot_description}],
        condition=UnlessCondition(is_sim)
    )

    # FIX: Pass the YAML via --ros-args --params-file inside `arguments`, NOT
    # inside `parameters=[]`. When mixed with a dict in `parameters=[]`, Humble's
    # controller_manager silently drops the per-controller blocks (joints,
    # command_interfaces). Using --ros-args --params-file loads the YAML directly
    # into the node's parameter server before any controller is configured.
    controller_manager = Node(
        package="controller_manager",
        executable="ros2_control_node",
        name="controller_manager",
        output="screen",
        parameters=[
            {"robot_description": robot_description},
            {"use_sim_time": False},   # hardcoded False — this node only runs on real robot
        ],
        arguments=[
            "--ros-args",
            "--params-file", controllers_yaml
        ],
        condition=UnlessCondition(is_sim),
    )

    # joint_state_broadcaster publishes /joint_states from hardware state interfaces.
    # Must be active before trajectory controllers start.
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

    # Controls joint_1, joint_2, joint_3 via JointTrajectoryController.
    arm_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        output="screen",
        arguments=[
            "arm_controller",
            "--controller-manager", "/controller_manager",
            "--controller-manager-timeout", "30",
        ]
    )

    # Controls joint_4 (joint_5 is a mimic, handled by ros2_control automatically).
    gripper_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        output="screen",
        arguments=[
            "gripper_controller",
            "--controller-manager", "/controller_manager",
            "--controller-manager-timeout", "30",
        ]
    )

    # Staggered delays for Raspberry Pi startup time:
    #   5s  — gives controller_manager time to fully initialize
    #   9s  — joint_state_broadcaster must be active before arm starts
    #   12s — arm must be active before gripper starts
    return LaunchDescription([
        is_sim_arg,
        robot_state_publisher_node,
        controller_manager,
        TimerAction(period=5.0,  actions=[joint_state_broadcaster_spawner]),
        TimerAction(period=9.0,  actions=[arm_controller_spawner]),
        TimerAction(period=12.0, actions=[gripper_controller_spawner]),
    ])