#include "../include/manipulator_controller/manipulator_interface.hpp"
#include <hardware_interface/types/hardware_interface_type_values.hpp>
#include <pluginlib/class_list_macros.hpp>


namespace manipulator_controller
{

//This function returns leading zeros so that a number becomes 3 digits long when converted to a string.
std::string compensateZeros(const int value)
{
  std::string compensate_zeros = "";
  if(value < 10){
    compensate_zeros = "00";
  } else if(value < 100){
    compensate_zeros = "0";
  } else {
    compensate_zeros = "";
  }
  return compensate_zeros;
}


// Constructor - A special member function that is called automatically when an object is created.
ManipulatorInterface::ManipulatorInterface()
{
}

// Destructor - A special member function that is called automatically when an object is destroyed.
ManipulatorInterface::~ManipulatorInterface()
{
  if (arduino_.IsOpen())
  {
    try  // LibSerial::SerialPort::Close() can throw exceptions. Destructors are a dangerous place for exceptions.
    {
      arduino_.Close();
    }
    catch (...) // Catch any exception that might occur during the closing of the serial port.
    {
      // RCLCPP_FATAL_STREAM is a ROS 2 logging macro used to log very serious, unrecoverable errors. RCLCPP_FATAL_STREAM(logger, message_stream)
      RCLCPP_FATAL_STREAM(rclcpp::get_logger("ManipulatorInterface"),
                          "Something went wrong while closing connection with port " << port_);
    }
  }
}


CallbackReturn ManipulatorInterface::on_init(const hardware_interface::HardwareInfo &hardware_info)  //hardware_info contains all the configuration details needed to set up the hardware interface.
{
  CallbackReturn result = hardware_interface::SystemInterface::on_init(hardware_info);  // You are calling the base class implementation of on_init().
  if (result != CallbackReturn::SUCCESS)  // If the base class initialization fails, you return the failure result immediately.
  {
    return result;
  }

  try
  {
    port_ = info_.hardware_parameters.at("port");  // .at() - If the key "port" does not exist in the map, it throws an std::out_of_range exception.
  }
  catch (const std::out_of_range &e) // Catching the specific exception that might be thrown if the "port" parameter is missing.
  {
    RCLCPP_FATAL(rclcpp::get_logger("ManipulatorInterface"), "No Serial Port provided! Aborting");
    return CallbackReturn::FAILURE;
  }

  // Reserve space in the vectors to hold position commands and states for each joint.
  // info_.joints.size() - gives the number of joints defined in the hardware_info.
  position_commands_.reserve(info_.joints.size()); // Reserve space for position commands. reserve(4) = 4 doubles( 8 bytes each) = 32 bytes
  position_states_.reserve(info_.joints.size());
  prev_position_commands_.reserve(info_.joints.size()); // Reserve space for previous position commands.

  return CallbackReturn::SUCCESS;
}

// This function exports the state interfaces for the manipulator hardware. - “What state values can controllers read from this hardware?”
std::vector<hardware_interface::StateInterface> ManipulatorInterface::export_state_interfaces() // std::vector<hardware_interface::StateInterface> - return type - (joint name, interface type, memory address)
{
  std::vector<hardware_interface::StateInterface> state_interfaces; // Create an empty vector to hold the state interfaces. this vector will be filled and returned to the controller manager.

  // Provide only a position Interafce
  for (size_t i = 0; i < info_.joints.size(); i++)  // size_t - unsigned integer type used for sizes and counts. info_.joints.size() - number of joints defined in the hardware_info.
  {
    //creating one StateInterface for each joint and adding them to the state_interfaces vector.
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &position_states_[i])); //(joint name, HW IF type, pointer to the internal memory, where the state value is stored) ex: joint_1, position, &position_states_[0]
  }  // "For joint i, the position command lives at memory address &position_commands_[i]."

  return state_interfaces;
}


std::vector<hardware_interface::CommandInterface> ManipulatorInterface::export_command_interfaces()
{
  std::vector<hardware_interface::CommandInterface> command_interfaces;

  // Provide only a position Interafce
  for (size_t i = 0; i < info_.joints.size(); i++)
  {
    command_interfaces.emplace_back(hardware_interface::CommandInterface(     // emplace_back - constructs a new CommandInterface object in place at the end of the vector.
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &position_commands_[i]));
  }

  return command_interfaces;
}


// This function is called when the hardware interface is activated.
CallbackReturn ManipulatorInterface::on_activate(const rclcpp_lifecycle::State &previous_state)
{
  RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterface"), "Starting robot hardware ...");

  // Reset commands and states
  position_commands_ = { 0.0, 0.0, 0.0, 0.0 };
  prev_position_commands_ = { 0.0, 0.0, 0.0, 0.0 };
  position_states_ = { 0.0, 0.0, 0.0, 0.0 };

  try
  {
    arduino_.Open(port_);
    arduino_.SetBaudRate(LibSerial::BaudRate::BAUD_115200);
  }
  catch (...)
  {
    RCLCPP_FATAL_STREAM(rclcpp::get_logger("ManipulatorInterface"),
                        "Something went wrong while interacting with port " << port_);
    return CallbackReturn::FAILURE;
  }

  RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterface"),
              "Hardware started, ready to take commands");
  return CallbackReturn::SUCCESS;
}

// This function is called when the hardware interface is deactivated.
CallbackReturn ManipulatorInterface::on_deactivate(const rclcpp_lifecycle::State &previous_state)
{
  RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterface"), "Stopping robot hardware ...");

  if (arduino_.IsOpen())
  {
    try
    {
      arduino_.Close();
    }
    catch (...)
    {
      RCLCPP_FATAL_STREAM(rclcpp::get_logger("ManipulatorInterface"),
                          "Something went wrong while closing connection with port " << port_);
    }
  }

  RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterface"), "Hardware stopped");
  return CallbackReturn::SUCCESS;
}


hardware_interface::return_type ManipulatorInterface::read(const rclcpp::Time &time,
                                                          const rclcpp::Duration &period)
{
  // Open Loop Control - assuming the robot is always where we command to be
  position_states_ = position_commands_;
  return hardware_interface::return_type::OK;
}



hardware_interface::return_type ManipulatorInterface::write(const rclcpp::Time &time,
                                                           const rclcpp::Duration &period)
{
  if (position_commands_ == prev_position_commands_)
  {
    // Nothing changed, do not send any command
    return hardware_interface::return_type::OK;
  }

  std::string msg;  // This will hold the full command string sent to Arduino.
  // static_cast<int> explicitly converts a value to an int at compile time.
  int base = static_cast<int>(((position_commands_.at(0) + (M_PI / 2)) * 180) / M_PI);  // deg = rad × (180 / π), Also shift by + π/2 to match servo zero position
  msg.append("b");
  msg.append(compensateZeros(base));
  msg.append(std::to_string(base));
  msg.append(",");
  int shoulder = 180 - static_cast<int>(((position_commands_.at(1) + (M_PI / 2)) * 180) / M_PI);
  msg.append("s");
  msg.append(compensateZeros(shoulder));
  msg.append(std::to_string(shoulder));
  msg.append(",");
  int elbow = static_cast<int>(((position_commands_.at(2) + (M_PI / 2)) * 180) / M_PI);
  msg.append("e");
  msg.append(compensateZeros(elbow));
  msg.append(std::to_string(elbow));
  msg.append(",");
  int gripper = static_cast<int>(((-position_commands_.at(3)) * 180) / (M_PI / 2));
  msg.append("g");
  msg.append(compensateZeros(gripper));
  msg.append(std::to_string(gripper));
  msg.append(",");

  try
  {
    RCLCPP_INFO_STREAM(rclcpp::get_logger("ManipulatorInterface"), "Sending new command " << msg);
    arduino_.Write(msg);
  }
  catch (...)
  {
    RCLCPP_ERROR_STREAM(rclcpp::get_logger("ManipulatorInterface"),
                        "Something went wrong while sending the message "
                            << msg << " to the port " << port_);
    return hardware_interface::return_type::ERROR;
  }

  prev_position_commands_ = position_commands_;

  return hardware_interface::return_type::OK;
}
}  // namespace manipulator_controller

PLUGINLIB_EXPORT_CLASS(manipulator_controller::ManipulatorInterface, hardware_interface::SystemInterface)
// This macro registers your C++ class as a ROS 2 plugin so it can be loaded at runtime.
// PLUGINLIB_EXPORT_CLASS(class_type, base_class_type)
// class_type - The fully qualified name of the class you are registering as a plugin.
// base_class_type - The fully qualified name of the base class type that your class inherits from
// controller_manager loads it dynamically using a string name from URDF