from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():

    use_gui_arg = DeclareLaunchArgument(
        'use_gui',
        default_value='false',
        description='Launch RViz and other GUIs (set false for headless Raspberry Pi)'
    )

    use_gui = LaunchConfiguration('use_gui')

    controller = IncludeLaunchDescription(
        os.path.join(
            get_package_share_directory("manipulator_controller"),
            "launch",
            "controller.launch.py"
        ),
        launch_arguments={"is_sim": "False"}.items()
    )

    moveit = IncludeLaunchDescription(
        os.path.join(
            get_package_share_directory("manipulator_moveit"),
            "launch",
            "moveit.launch.py"
        ),
        launch_arguments={"is_sim": "False", "use_gui": use_gui}.items()
    )

    remote_interface = IncludeLaunchDescription(
        os.path.join(
            get_package_share_directory("manipulator_remote"),
            "launch",
            "remote_interface.launch.py"
        ),
        launch_arguments={"is_sim": "False"}.items()
    )

    return LaunchDescription([
        use_gui_arg,
        controller,
        moveit,
        remote_interface
    ])


# First connect the Arduino to the computer and upload the firmware using the Arduino IDE.

# cd FYP/My_Work/Repo/Robot_Manipulator
# colcon build

# . install/setup.bash
# ros2 launch manipulator_remote real_robot.launch.py

# . install/setup.bash
# ros2 action list
# ros2 action send_goal /task_server manipulator_msgs/action/ManipulatorTask "{task_number: 1}" 