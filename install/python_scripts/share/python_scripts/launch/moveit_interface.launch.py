import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
from moveit_configs_utils import MoveItConfigsBuilder

#MoveItConfigsBuilder: 
    #Collects all MoveIt configuration files, like URDF, SRDF, controllers, kinematics, planning pipelines, etc. 
    #Converts them into ROS parameters that MoveIt nodes can use.
    #Feeds them to nodes (like MoveItPy)

def generate_launch_description():

    moveit_config = (
        MoveItConfigsBuilder("manipulator", package_name="manipulator_moveit") # "manipulator" - robot name. manipulator_moveit - moveit config package. "Load configs for the robot called manipulator"
        .robot_description( file_path=os.path.join(
            get_package_share_directory("manipulator_description"), 
            "urdf", 
            "manipulator.urdf.xacro"
            ) 
        ) # Load URDF from manipulator_description package
        .robot_description_semantic( file_path="config/manipulator.srdf" ) # Load SRDF from manipulator_moveit package
        .trajectory_execution( file_path="config/moveit_controllers.yaml" ) # Load controller config (which controllers to use for which joints)
        .moveit_cpp( file_path="config/planning_python_api.yaml" )  # Load planning pipeline config for MoveItPy
        .to_moveit_configs() # bundles everything together. Finalize the MoveItConfigs object
    )

    # Define the MoveIt interface node
    moveit_interface = Node(    
        package="python_scripts",   # Name of your package containing moveit_interface.py
        executable="moveit_interface",  # Name of the executable (script) to run
        parameters=[moveit_config.to_dict(),
                    {"use_sim_time": True}]
    )

    return LaunchDescription([
        moveit_interface
    ])