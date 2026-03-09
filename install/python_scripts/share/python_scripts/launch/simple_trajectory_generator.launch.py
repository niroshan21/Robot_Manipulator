#!/usr/bin/env python3

import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, TimerAction
from launch.substitutions import LaunchConfiguration
from launch.conditions import IfCondition
from launch_ros.actions import Node
from moveit_configs_utils import MoveItConfigsBuilder


def generate_launch_description():
    """
    Simple Trajectory Generator Launch File
    
    This launches:
    1. MoveGroup (for motion planning)
    2. RViz2 (optional - for visualization)
    3. Simple trajectory generator (your custom trajectory)
    
    Usage:
        ros2 launch python_scripts simple_trajectory_generator.launch.py
    
    Or with custom name:
        ros2 launch python_scripts simple_trajectory_generator.launch.py trajectory_name:=my_traj
    """
    
    # Declare launch arguments
    is_sim_arg = DeclareLaunchArgument(
        'is_sim',
        default_value='true',
        description='Whether to use simulation time'
    )
    
    trajectory_name_arg = DeclareLaunchArgument(
        'trajectory_name',
        default_value='my_custom_trajectory',
        description='Name for the saved trajectory file'
    )
    
    save_directory_arg = DeclareLaunchArgument(
        'save_directory',
        default_value='savedTrajectories',
        description='Directory to save trajectories (relative to workspace root)'
    )
    
    use_rviz_arg = DeclareLaunchArgument(
        'use_rviz',
        default_value='false',
        description='Whether to launch RViz2 for visualization'
    )

    # Get launch configurations
    is_sim = LaunchConfiguration('is_sim')
    trajectory_name = LaunchConfiguration('trajectory_name')
    save_directory = LaunchConfiguration('save_directory')
    use_rviz = LaunchConfiguration('use_rviz')

    # Build MoveIt configuration
    moveit_config = (
        MoveItConfigsBuilder("manipulator", package_name="manipulator_moveit")
        .robot_description(file_path=os.path.join(
            get_package_share_directory("manipulator_description"), 
            "urdf", 
            "manipulator.urdf.xacro"
        ))
        .robot_description_semantic(file_path="config/manipulator.srdf")
        .trajectory_execution(file_path="config/moveit_controllers.yaml")
        .planning_pipelines(pipelines=["ompl", "chomp", "pilz_industrial_motion_planner", "stomp"])
        .moveit_cpp(file_path="config/planning_python_api.yaml")
        .to_moveit_configs()
    )

    # Move Group Node - Core planning node
    move_group_node = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        output="screen",
        parameters=[
            moveit_config.to_dict(),
            {"use_sim_time": is_sim},
            {"publish_robot_description_semantic": True},
        ],
        arguments=["--ros-args", "--log-level", "warn"],  # Less verbose
    )

    # RViz2 Node - Visualization (optional)
    rviz_config = os.path.join(
        get_package_share_directory("manipulator_moveit"), "config", "moveit.rviz"
    )

    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        output="screen",
        arguments=["-d", rviz_config],
        parameters=[
            moveit_config.robot_description,
            moveit_config.robot_description_semantic,
            moveit_config.robot_description_kinematics,
            moveit_config.joint_limits,
            {"use_sim_time": is_sim},
        ],
        condition=IfCondition(use_rviz)
    )
    
    # Simple Trajectory Generator Node
    trajectory_generator_node = Node(
        package='python_scripts',
        executable='simple_trajectory_generator',
        name='simple_trajectory_generator',
        output='screen',
        parameters=[
            moveit_config.to_dict(),
            {'use_sim_time': is_sim},
            {'trajectory_name': trajectory_name},
            {'save_directory': save_directory},
        ]
    )
    
    # Delay the trajectory generator to ensure move_group is fully ready
    delayed_trajectory_generator = TimerAction(
        period=5.0,  # Wait 5 seconds for move_group to initialize
        actions=[trajectory_generator_node]
    )
    
    return LaunchDescription([
        # Arguments
        is_sim_arg,
        trajectory_name_arg,
        save_directory_arg,
        use_rviz_arg,
        
        # Nodes
        move_group_node,
        rviz_node,
        delayed_trajectory_generator,
    ])
