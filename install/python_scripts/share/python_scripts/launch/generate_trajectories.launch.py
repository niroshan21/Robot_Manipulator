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
    Launch file to generate and save trajectories using MoveIt
    
    This single launch file will:
    1. Start MoveGroup for motion planning
    2. Start RViz2 for visualization
    3. Run the trajectory generator to create and save trajectories
    """
    
    # Declare launch arguments
    is_sim_arg = DeclareLaunchArgument(
        'is_sim',
        default_value='true',
        description='Whether to use simulation time'
    )
    
    num_trajectories_arg = DeclareLaunchArgument(
        'num_trajectories',
        default_value='5',
        description='Number of trajectories to generate'
    )
    
    save_directory_arg = DeclareLaunchArgument(
        'save_directory',
        default_value='savedTrajectories',
        description='Directory to save trajectories (relative to workspace root)'
    )
    
    use_rviz_arg = DeclareLaunchArgument(
        'use_rviz',
        default_value='true',
        description='Whether to launch RViz2'
    )

    # Get launch configurations
    is_sim = LaunchConfiguration('is_sim')
    num_trajectories = LaunchConfiguration('num_trajectories')
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
        arguments=["--ros-args", "--log-level", "info"],
    )

    # RViz2 Node - Visualization
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
    
    # Trajectory Generator Node
    trajectory_generator_node = Node(
        package='python_scripts',
        executable='generate_trajectories',
        name='trajectory_generator',
        output='screen',
        parameters=[
            moveit_config.to_dict(),
            {'use_sim_time': is_sim},
            {'num_trajectories': num_trajectories},
            {'save_directory': save_directory},
            {'planning_group': 'arm'},
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
        num_trajectories_arg,
        save_directory_arg,
        use_rviz_arg,
        
        # Nodes
        move_group_node,
        rviz_node,
        delayed_trajectory_generator,
    ])
