import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    """
    Lightweight remote interface launch file for real robot (RPI).
    Does NOT launch MoveIt - uses pre-generated trajectories instead.
    """

    is_sim_arg = DeclareLaunchArgument(
        "is_sim", 
        default_value="False"
    )
    
    trajectory_dir_arg = DeclareLaunchArgument(
        "trajectory_dir",
        default_value="savedTrajectories",
        description="Directory containing pre-generated trajectory JSON files"
    )

    is_sim = LaunchConfiguration("is_sim")
    trajectory_dir = LaunchConfiguration("trajectory_dir")

    # Lightweight task server - no MoveIt required
    task_server_node = Node(
        package="manipulator_remote",
        executable="task_server_lightweight.py",
        parameters=[
            {"use_sim_time": is_sim},
            {"trajectory_dir": trajectory_dir}
        ],
        output="screen"
    )

    return LaunchDescription([
        is_sim_arg,
        trajectory_dir_arg,
        task_server_node,
    ])


# Usage on Raspberry Pi:
# cd ~/Robot_Manipulator
# . install/setup.bash
# ros2 launch manipulator_remote remote_interface_lightweight.launch.py

# Then send tasks:
# ros2 action send_goal /task_server manipulator_msgs/action/ManipulatorTask "{task_number: 0}"
# ros2 action send_goal /task_server manipulator_msgs/action/ManipulatorTask "{task_number: 1}"
# ros2 action send_goal /task_server manipulator_msgs/action/ManipulatorTask "{task_number: 2}"
