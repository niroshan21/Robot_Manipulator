from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    """
    Lightweight launch file for real robot (Raspberry Pi).
    Uses pre-generated trajectories instead of MoveIt for reduced resource usage.
    """

    controller = IncludeLaunchDescription(
        os.path.join(
            get_package_share_directory("manipulator_controller"),
            "launch",
            "controller.launch.py"
        ),
        launch_arguments={"is_sim": "False"}.items()
    )

    # Use lightweight remote interface (no MoveIt required)
    remote_interface = IncludeLaunchDescription(
        os.path.join(
            get_package_share_directory("manipulator_remote"),
            "launch",
            "remote_interface_lightweight.launch.py"
        ),
        launch_arguments={"is_sim": "False"}.items()
    )

    return LaunchDescription([
        controller,
        remote_interface
    ])


# First connect the Arduino to the computer and upload the firmware using the Arduino IDE.

# cd ~/Robot_Manipulator
# colcon build --packages-select manipulator_msgs manipulator_description manipulator_controller manipulator_remote

# . install/setup.bash
# ros2 launch manipulator_remote real_robot_lightweight.launch.py

# In another terminal, send task commands:
# . install/setup.bash
# ros2 action send_goal /task_server manipulator_msgs/action/ManipulatorTask "{task_number: 0}" 
# ros2 action send_goal /task_server manipulator_msgs/action/ManipulatorTask "{task_number: 1}" 
# ros2 action send_goal /task_server manipulator_msgs/action/ManipulatorTask "{task_number: 2}" 
