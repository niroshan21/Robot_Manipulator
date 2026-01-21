#ifndef MANIPULATOR_INTERFACE_H
#define MANIPULATOR_INTERFACE_H

#include <rclcpp/rclcpp.hpp>    // ROS 2 C++ API, including rclcpp::Time and rclcpp::Duration
#include <hardware_interface/system_interface.hpp> // Base class for hardware interfaces
#include <libserial/SerialPort.h>   // Library for serial port communication
#include <rclcpp_lifecycle/state.hpp>  // Lifecycle state management - state means the state of the lifecycle node
#include <rclcpp_lifecycle/node_interfaces/lifecycle_node_interface.hpp>  // Lifecycle node interface - provides lifecycle capabilities to nodes

#include <vector>
#include <string>


namespace manipulator_controller
{

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;   //This creates a shortcut name for a long type. It’s used in your class to tell ROS2 whether a lifecycle step succeeded or failed.

class ManipulatorInterface : public hardware_interface::SystemInterface    // Inheriting from SystemInterface to implement hardware interface functionalities. “My manipulator is a ROS2-controlled hardware system.” 
{
public:
  ManipulatorInterface();   // Constructor - initializes an instance of the ManipulatorInterface class.
  virtual ~ManipulatorInterface();  // Destructor - cleans up resources when an instance of the class is destroyed. virtual ensures polymorphic deletion works correctly.

  // Implementing rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface
  virtual CallbackReturn on_activate(const rclcpp_lifecycle::State &previous_state) override;  //on_activate is defined in SystemInterface as virtual. Your ManipulatorInterface class overrides it to provide specific behavior when the hardware is activated.
  virtual CallbackReturn on_deactivate(const rclcpp_lifecycle::State &previous_state) override;

  // Implementing hardware_interface::SystemInterface
  virtual CallbackReturn on_init(const hardware_interface::HardwareInfo &hardware_info) override; // Initializes the hardware interface with provided hardware information(like joint names, limits, parameters etc.)
  virtual std::vector<hardware_interface::StateInterface> export_state_interfaces() override;  //Returns a vector of StateInterface objects, which are basically readable robot states. Example: joint positions, velocities, or sensor readings.
  virtual std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;  //Returns a vector of CommandInterface objects, which are writable commands to control the robot. Example: joint position commands, velocity commands, or effort commands.
  virtual hardware_interface::return_type read(const rclcpp::Time & time, const rclcpp::Duration & period) override;  //Reads the current state of the hardware and updates the state interfaces accordingly.
  virtual hardware_interface::return_type write(const rclcpp::Time & time, const rclcpp::Duration & period) override;  //This function sends commands to the hardware, like moving a motor or actuator.

private:
  LibSerial::SerialPort arduino_;   // SerialPort object to manage serial communication with the manipulator hardware (e.g., an Arduino board).
  std::string port_;  // Serial port name (e.g., "/dev/ttyUSB0" or "COM3").
  std::vector<double> position_commands_;  // Vector to hold position commands for each joint of the manipulator.
  std::vector<double> prev_position_commands_;   // Vector to hold previous position commands for each joint of the manipulator.
  std::vector<double> position_states_;  // Vector to hold current position states for each joint of the manipulator.
};
}  // namespace manipulator_controller


#endif  // MANIPULATOR_INTERFACE_H
