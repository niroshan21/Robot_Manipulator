import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch.conditions import IfCondition, UnlessCondition
from launch_ros.actions import Node
from moveit_configs_utils import MoveItConfigsBuilder


def generate_launch_description():

    is_sim_arg = DeclareLaunchArgument(
        "is_sim", 
        default_value="True"
    )
    
    use_python_arg = DeclareLaunchArgument(
        "use_python", 
        default_value="True"
    )

    is_sim = LaunchConfiguration("is_sim")
    use_python = LaunchConfiguration("use_python")

    moveit_config = (
        MoveItConfigsBuilder("manipulator", package_name="manipulator_moveit")
        .robot_description( file_path=os.path.join(
            get_package_share_directory("manipulator_description"), 
            "urdf", 
            "manipulator.urdf.xacro"
            ) 
        )
        .robot_description_semantic( file_path="config/manipulator.srdf" )
        .trajectory_execution( file_path="config/moveit_controllers.yaml" )
        .moveit_cpp( file_path="config/planning_python_api.yaml" )
        .to_moveit_configs()
    )

    task_servet_node_py = Node(
        package="manipulator_remote",
        executable="task_server.py",
        condition=IfCondition(use_python),
        parameters=[moveit_config.to_dict(),
                    {"use_sim_time": is_sim}]
    )

    # task_servet_node_cpp = Node(
    #     package="manipulator_remote",
    #     executable="task_server_node_cpp",
    #     condition=UnlessCondition(use_python),
    #     parameters=[{"use_sim_time": is_sim}]
    # )

    return LaunchDescription([
        is_sim_arg,
        use_python_arg,
        task_servet_node_py,
        # task_servet_node_cpp
    ])




# cd FYP/My_Work/Repo/Robot_Manipulator
# colcon build

# . install/setup.bash
# ros2 launch manipulator_description gazebo.launch.py

# . install/setup.bash
# ros2 launch manipulator_controller controller.launch.py

# . install/setup.bash
# ros2 launch manipulator_moveit moveit.launch.py

# . install/setup.bash
# ros2 launch manipulator_remote remote_interface.launch.py use_python:=True is_sim:=True

# In another terminal, run the action client
# ros2 action list
# . install/setup.bash
# ros2 action send_goal /task_server manipulator_msgs/action/ManipulatorTask "{task_number: 1}" 