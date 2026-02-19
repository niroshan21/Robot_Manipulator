#include "../include/manipulator_controller/manipulator_interface_gpio.hpp"
#include <hardware_interface/types/hardware_interface_type_values.hpp>
#include <pluginlib/class_list_macros.hpp>
#include <cmath>


namespace manipulator_controller
{

ManipulatorInterfaceGPIO::ManipulatorInterfaceGPIO()
  : gpio_initialized_(false)
{
}

ManipulatorInterfaceGPIO::~ManipulatorInterfaceGPIO()
{
  if (gpio_initialized_)
  {
    // Stop all PWM signals
    for (int pin : gpio_pins_)
    {
      gpioServo(pin, 0);  // Turn off servo
    }
    
    // Terminate pigpio
    gpioTerminate();
    gpio_initialized_ = false;
  }
}

CallbackReturn ManipulatorInterfaceGPIO::on_init(const hardware_interface::HardwareInfo &hardware_info)
{
  CallbackReturn result = hardware_interface::SystemInterface::on_init(hardware_info);
  if (result != CallbackReturn::SUCCESS)
  {
    return result;
  }

  // Get GPIO pin assignments from hardware parameters
  try
  {
    gpio_pins_[0] = std::stoi(info_.hardware_parameters.at("gpio_base"));
    gpio_pins_[1] = std::stoi(info_.hardware_parameters.at("gpio_shoulder"));
    gpio_pins_[2] = std::stoi(info_.hardware_parameters.at("gpio_elbow"));
    gpio_pins_[3] = std::stoi(info_.hardware_parameters.at("gpio_gripper"));
  }
  catch (const std::out_of_range &e)
  {
    RCLCPP_FATAL(rclcpp::get_logger("ManipulatorInterfaceGPIO"), 
                 "GPIO pin parameters not provided! Need: gpio_base, gpio_shoulder, gpio_elbow, gpio_gripper");
    return CallbackReturn::FAILURE;
  }
  catch (const std::invalid_argument &e)
  {
    RCLCPP_FATAL(rclcpp::get_logger("ManipulatorInterfaceGPIO"), 
                 "Invalid GPIO pin number provided!");
    return CallbackReturn::FAILURE;
  }

  // Reserve space for joint states and commands
  position_commands_.reserve(info_.joints.size());
  position_states_.reserve(info_.joints.size());
  prev_position_commands_.reserve(info_.joints.size());

  RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterfaceGPIO"), 
              "GPIO pins configured - Base: %d, Shoulder: %d, Elbow: %d, Gripper: %d",
              gpio_pins_[0], gpio_pins_[1], gpio_pins_[2], gpio_pins_[3]);

  return CallbackReturn::SUCCESS;
}

std::vector<hardware_interface::StateInterface> ManipulatorInterfaceGPIO::export_state_interfaces()
{
  std::vector<hardware_interface::StateInterface> state_interfaces;

  for (size_t i = 0; i < info_.joints.size(); i++)
  {
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &position_states_[i]));
  }

  return state_interfaces;
}

std::vector<hardware_interface::CommandInterface> ManipulatorInterfaceGPIO::export_command_interfaces()
{
  std::vector<hardware_interface::CommandInterface> command_interfaces;

  for (size_t i = 0; i < info_.joints.size(); i++)
  {
    command_interfaces.emplace_back(hardware_interface::CommandInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &position_commands_[i]));
  }

  return command_interfaces;
}

CallbackReturn ManipulatorInterfaceGPIO::on_activate(const rclcpp_lifecycle::State &previous_state)
{
  RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterfaceGPIO"), "Initializing GPIO hardware...");

  // Initialize pigpio library
  if (gpioInitialise() < 0)
  {
    RCLCPP_FATAL(rclcpp::get_logger("ManipulatorInterfaceGPIO"), 
                 "Failed to initialize pigpio! Make sure pigpiod daemon is running.");
    return CallbackReturn::FAILURE;
  }
  gpio_initialized_ = true;

  // Configure GPIO pins as outputs and set to neutral position (90 degrees for MG995)
  for (int pin : gpio_pins_)
  {
    gpioSetMode(pin, PI_OUTPUT);
    
    // Set initial position to 90 degrees (neutral/center position)
    // Using gpioServo which handles the PWM automatically at 50Hz
    // MG995 servos: 1500us = 90 degrees (center)
    
    if (gpioServo(pin, PWM_CENTER) != 0)
    {
      RCLCPP_ERROR(rclcpp::get_logger("ManipulatorInterfaceGPIO"), 
                   "Failed to set servo on GPIO pin %d", pin);
      return CallbackReturn::FAILURE;
    }
    
    RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterfaceGPIO"), 
                "GPIO pin %d initialized with neutral position (MG995)", pin);
  }

  // Initialize position vectors to neutral position (0.0 radians)
  position_commands_ = { 0.0, 0.0, 0.0, 0.0 };
  prev_position_commands_ = { 0.0, 0.0, 0.0, 0.0 };
  position_states_ = { 0.0, 0.0, 0.0, 0.0 };

  RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterfaceGPIO"), 
              "GPIO hardware initialized, ready to take commands");
  
  // Small delay to allow servos to reach initial position
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  
  return CallbackReturn::SUCCESS;
}

CallbackReturn ManipulatorInterfaceGPIO::on_deactivate(const rclcpp_lifecycle::State &previous_state)
{
  RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterfaceGPIO"), "Deactivating GPIO hardware...");

  if (gpio_initialized_)
  {
    // Stop all PWM signals
    for (int pin : gpio_pins_)
    {
      gpioServo(pin, 0);  // Turn off servo PWM
    }
    
    gpioTerminate();
    gpio_initialized_ = false;
  }

  RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterfaceGPIO"), "GPIO hardware deactivated");
  return CallbackReturn::SUCCESS;
}

hardware_interface::return_type ManipulatorInterfaceGPIO::read(const rclcpp::Time &time,
                                                               const rclcpp::Duration &period)
{
  // Open Loop Control - assuming the robot is always where we command it to be
  // In a real system with encoders, you would read actual positions here
  position_states_ = position_commands_;
  return hardware_interface::return_type::OK;
}

hardware_interface::return_type ManipulatorInterfaceGPIO::write(const rclcpp::Time &time,
                                                                const rclcpp::Duration &period)
{
  // Only write if commands have changed
  if (position_commands_ == prev_position_commands_)
  {
    return hardware_interface::return_type::OK;
  }

  if (!gpio_initialized_)
  {
    RCLCPP_ERROR(rclcpp::get_logger("ManipulatorInterfaceGPIO"), "GPIO not initialized!");
    return hardware_interface::return_type::ERROR;
  }

  // Convert radians to servo angles (0-180 degrees) and then to PWM pulse widths
  // Following the same mapping as the Arduino version
  
  // Base joint (joint_1)
  int base_angle = 180 - static_cast<int>(((position_commands_[0] + (M_PI / 2)) * 180) / M_PI);
  base_angle = std::clamp(base_angle, 0, 180);
  int base_pulse = angleToPulseWidth(base_angle * M_PI / 180.0, 0, M_PI);
  
  // Shoulder joint (joint_2)
  int shoulder_angle = 180 - static_cast<int>(((position_commands_[1] + (M_PI / 2)) * 180) / M_PI);
  shoulder_angle = std::clamp(shoulder_angle, 0, 180);
  int shoulder_pulse = angleToPulseWidth(shoulder_angle * M_PI / 180.0, 0, M_PI);
  
  // Elbow joint (joint_3)
  int elbow_angle = 180 - static_cast<int>(((position_commands_[2] + (M_PI / 2)) * 180) / M_PI);
  elbow_angle = std::clamp(elbow_angle, 0, 180);
  int elbow_pulse = angleToPulseWidth(elbow_angle * M_PI / 180.0, 0, M_PI);
  
  // Gripper joint (joint_4)
  int gripper_angle = static_cast<int>(((-position_commands_[3]) * 180) / (M_PI / 2));
  gripper_angle = std::clamp(gripper_angle, 0, 180);
  int gripper_pulse = angleToPulseWidth(gripper_angle * M_PI / 180.0, 0, M_PI);

  // Set servo positions via GPIO PWM
  gpioServo(gpio_pins_[0], base_pulse);
  gpioServo(gpio_pins_[1], shoulder_pulse);
  gpioServo(gpio_pins_[2], elbow_pulse);
  gpioServo(gpio_pins_[3], gripper_pulse);

  RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterfaceGPIO"), 
              "Servo positions - Base: %d°(%dus), Shoulder: %d°(%dus), Elbow: %d°(%dus), Gripper: %d°(%dus)",
              base_angle, base_pulse, shoulder_angle, shoulder_pulse, 
              elbow_angle, elbow_pulse, gripper_angle, gripper_pulse);

  prev_position_commands_ = position_commands_;

  return hardware_interface::return_type::OK;
}

int ManipulatorInterfaceGPIO::angleToPulseWidth(double angle, double min_angle, double max_angle)
{
  // Map angle to 0-180 degree range
  double normalized = (angle - min_angle) / (max_angle - min_angle);
  normalized = std::clamp(normalized, 0.0, 1.0);
  
  // Map to PWM pulse width (500us = 0°, 2500us = 180°)
  int pulse_width = PWM_MIN + static_cast<int>(normalized * (PWM_MAX - PWM_MIN));
  
  return std::clamp(pulse_width, PWM_MIN, PWM_MAX);
}

}  // namespace manipulator_controller

PLUGINLIB_EXPORT_CLASS(manipulator_controller::ManipulatorInterfaceGPIO, hardware_interface::SystemInterface)
