#include "../include/manipulator_controller/manipulator_interface_sts3215.hpp"
#include <hardware_interface/types/hardware_interface_type_values.hpp>
//This header provides a small set of string constants. The most important ones are:
    // hardware_interface::HW_IF_POSITION  = "position"
    // hardware_interface::HW_IF_VELOCITY = "velocity"
    // hardware_interface::HW_IF_EFFORT  = "effort"

#include <pluginlib/class_list_macros.hpp>
// This macro exports the class as a plugin, making it discoverable at runtime by the ROS 2 framework. The first argument is the package name, and the second is the fully qualified name of the class.

#include <algorithm>    // Provides generic algorithms that work on containers and ranges. (std::clamp, std::min, std::max)
#include <cmath>        // Math functions and constants. (For example, std::abs, std::sin, std::cos, std::tan, std::atan2, M_PI)
#include <cctype>       // std::isspace
#include <sstream>      // For string stream operations, which allow you to build strings from other data types in a convenient way. (std::stringstream)
#include <pigpiod_if2.h>

namespace
{
bool parseFiniteDouble(const std::string &text, double &out_value)
{
  if (text.empty())
  {
    return false;
  }

  try
  {
    size_t idx = 0;
    double value = std::stod(text, &idx);

    while (idx < text.size() && std::isspace(static_cast<unsigned char>(text[idx])))
    {
      ++idx;
    }

    if (idx != text.size() || !std::isfinite(value))
    {
      return false;
    }

    out_value = value;
    return true;
  }
  catch (...)
  {
    return false;
  }
}
}  // namespace

namespace manipulator_controller
{

// Constructor values = fallback defaults.
// URDF values = your actual configuration for this robot; they override the defaults.
// If you remove them from the URDF, the defaults in the code are used. If you keep them, the URDF wins.
ManipulatorInterfaceSTS3215::ManipulatorInterfaceSTS3215()
  : serial_open_(false),
    baudrate_(1000000),
    feedback_from_hardware_(false),
    warned_feedback_(false),
    goal_position_addr_(0x2A),
    present_position_addr_(0x38),
    position_raw_min_(0),
    position_raw_max_(4095),
    position_rad_min_(-M_PI / 2.0),
    position_rad_max_(M_PI / 2.0),
    servo_speed_(0),
    servo_acceleration_(0),
    tuning_enable_(false),
    has_position_p_gain_(false),
    has_position_d_gain_(false),
    has_position_i_gain_(false),
    has_deadband_cw_(false),
    has_deadband_ccw_(false),
    has_punch_(false),
    position_p_gain_(0),
    position_d_gain_(0),
    position_i_gain_(0),
    deadband_cw_(0),
    deadband_ccw_(0),
    punch_(0),
    has_velocity_p_gain_(false),
    has_velocity_i_gain_(false),
    velocity_p_gain_(0),
    velocity_i_gain_(0),
    sts_joint_count_(0),
    gpio_initialized_(false),
    gpio_gripper_pin_(22),
    pi_(-1)
{
}

ManipulatorInterfaceSTS3215::~ManipulatorInterfaceSTS3215()
{
  closePort();

  if (gpio_initialized_)
  {
    set_servo_pulsewidth(pi_, gpio_gripper_pin_, 0);
    pigpio_stop(pi_);
    gpio_initialized_ = false;
  }
}

// This function is called once at startup, before the robot is enabled. It's where you read the configuration from the URDF and do any necessary setup that doesn't involve talking to the hardware yet.
CallbackReturn ManipulatorInterfaceSTS3215::on_init(const hardware_interface::HardwareInfo &hardware_info)
{
  // Step 1 — you pass hardware_info TO the parent
  CallbackReturn result = hardware_interface::SystemInterface::on_init(hardware_info);  // Call the base class's on_init() to do common initialization. This also parses the URDF and fills in the info_ struct with the data.
  if (result != CallbackReturn::SUCCESS)  // If the base class initialization failed, we return failure and don't proceed with our specific initialization.
  {
    return result;
  }

  // Inside the parent, this happens:
  // info_ = hardware_info   ← parent copies it into the protected member

  try
  {
    // Step 2 — now info_ is available in YOUR class
    port_ = info_.hardware_parameters.at("port");

    // If key MISSING → throws an exception. throws std::out_of_range.
  }
  catch (const std::out_of_range &)
  {
    RCLCPP_FATAL(rclcpp::get_logger("ManipulatorInterfaceSTS3215"), "No serial port provided");
    return CallbackReturn::FAILURE;
  }

  // info_.hardware_parameters = {
  //    "port"          → "/dev/ttyUSB0"
  //    "baudrate"      → "1000000"
  //    "servo_ids"     → "1,2,3,4"
  //    "servo_speed"   → "1000"
  //    ...  }
  // Everything is a string because that's how URDF stores parameters

  // "baudrate" EXISTS → iterator points to {"baudrate", "1000000"}
  auto baudrate_it = info_.hardware_parameters.find("baudrate");
  // "baudrate" MISSING → iterator points to map.end() (a special "not found" marker)
  if (baudrate_it != info_.hardware_parameters.end())
  {
    baudrate_ = std::stoi(baudrate_it->second);  
    // baudrate_it->first   // "baudrate"     ← the key
    // baudrate_it->second  // "1000000"      ← the value as a string
    // std::stoi(...)       // 1000000        ← convert the string to an integer
  }

  auto feedback_it = info_.hardware_parameters.find("feedback_from_hardware");
  if (feedback_it != info_.hardware_parameters.end())
  {
    feedback_from_hardware_ = (feedback_it->second == "true");
  }

  auto servo_ids_it = info_.hardware_parameters.find("servo_ids");
  if (servo_ids_it != info_.hardware_parameters.end())
  {
    sts_joint_count_ = info_.joints.size() > 0 ? info_.joints.size() - 1 : 0;
    loadServoIds(servo_ids_it->second, sts_joint_count_);
  }
  else
  {
    sts_joint_count_ = info_.joints.size() > 0 ? info_.joints.size() - 1 : 0;
    loadServoIds("", sts_joint_count_);
  }

  has_position_p_gain_per_servo_.assign(sts_joint_count_, false);
  has_position_d_gain_per_servo_.assign(sts_joint_count_, false);
  has_position_i_gain_per_servo_.assign(sts_joint_count_, false);
  has_deadband_cw_per_servo_.assign(sts_joint_count_, false);
  has_deadband_ccw_per_servo_.assign(sts_joint_count_, false);
  has_punch_per_servo_.assign(sts_joint_count_, false);
  has_velocity_p_gain_per_servo_.assign(sts_joint_count_, false);
  has_velocity_i_gain_per_servo_.assign(sts_joint_count_, false);
  position_p_gain_per_servo_.assign(sts_joint_count_, 0);
  position_d_gain_per_servo_.assign(sts_joint_count_, 0);
  position_i_gain_per_servo_.assign(sts_joint_count_, 0);
  deadband_cw_per_servo_.assign(sts_joint_count_, 0);
  deadband_ccw_per_servo_.assign(sts_joint_count_, 0);
  punch_per_servo_.assign(sts_joint_count_, 0);
  velocity_p_gain_per_servo_.assign(sts_joint_count_, 0);
  velocity_i_gain_per_servo_.assign(sts_joint_count_, 0);

  auto gripper_pin_it = info_.hardware_parameters.find("gpio_gripper");
  if (gripper_pin_it != info_.hardware_parameters.end())
  {
    try
    {
      gpio_gripper_pin_ = std::stoi(gripper_pin_it->second);
    }
    catch (...)
    {
      RCLCPP_WARN(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                  "Invalid gpio_gripper pin; using default %d", gpio_gripper_pin_);
    }
  }

  auto goal_addr_it = info_.hardware_parameters.find("goal_position_addr");
  if (goal_addr_it != info_.hardware_parameters.end())
  {
    goal_position_addr_ = static_cast<uint8_t>(std::stoi(goal_addr_it->second, nullptr, 0));
    // nullptr = no pointer(dont track the number of chars read)  , 0 = auto-detect the number base (hex if starts with 0x, octal if starts with 0, decimal otherwise)
  }

  auto present_addr_it = info_.hardware_parameters.find("present_position_addr");
  if (present_addr_it != info_.hardware_parameters.end())
  {
    present_position_addr_ = static_cast<uint8_t>(std::stoi(present_addr_it->second, nullptr, 0));
  }

  auto raw_min_it = info_.hardware_parameters.find("position_raw_min");
  if (raw_min_it != info_.hardware_parameters.end())
  {
    position_raw_min_ = static_cast<uint16_t>(std::stoi(raw_min_it->second));
  }

  auto raw_max_it = info_.hardware_parameters.find("position_raw_max");
  if (raw_max_it != info_.hardware_parameters.end())
  {
    position_raw_max_ = static_cast<uint16_t>(std::stoi(raw_max_it->second));
  }

  double rad_min = position_rad_min_;
  auto rad_min_it = info_.hardware_parameters.find("position_rad_min");
  if (rad_min_it != info_.hardware_parameters.end())
  {
    parseFiniteDouble(rad_min_it->second, rad_min);
  }

  double rad_max = position_rad_max_;
  auto rad_max_it = info_.hardware_parameters.find("position_rad_max");
  if (rad_max_it != info_.hardware_parameters.end())
  {
    parseFiniteDouble(rad_max_it->second, rad_max);
  }

  if (rad_min < rad_max)
  {
    position_rad_min_ = rad_min;
    position_rad_max_ = rad_max;
  }
  else
  {
    RCLCPP_WARN(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                "Invalid position_rad_min/max; using defaults.");
  }

  auto speed_it = info_.hardware_parameters.find("servo_speed");
  if (speed_it != info_.hardware_parameters.end())
  {
    servo_speed_ = static_cast<uint16_t>(std::stoi(speed_it->second));
  }

  auto acc_it = info_.hardware_parameters.find("servo_acceleration");
  if (acc_it != info_.hardware_parameters.end())
  {
    servo_acceleration_ = static_cast<uint8_t>(std::stoi(acc_it->second));
  }

  auto tuning_it = info_.hardware_parameters.find("tuning_enable");
  if (tuning_it != info_.hardware_parameters.end())
  {
    tuning_enable_ = (tuning_it->second == "true");
  }

  auto read_tuning_param = [&](const char *key, int &value, bool &has_value)
  {
    auto it = info_.hardware_parameters.find(key);
    if (it == info_.hardware_parameters.end())
    {
      return;
    }
    try
    {
      value = std::stoi(it->second);
      has_value = true;
    }
    catch (...)
    {
      RCLCPP_WARN(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                  "Invalid %s value '%s'", key, it->second.c_str());
    }
  };

  read_tuning_param("position_p_gain", position_p_gain_, has_position_p_gain_);
  read_tuning_param("position_d_gain", position_d_gain_, has_position_d_gain_);
  read_tuning_param("position_i_gain", position_i_gain_, has_position_i_gain_);
  read_tuning_param("deadband_cw", deadband_cw_, has_deadband_cw_);
  read_tuning_param("deadband_ccw", deadband_ccw_, has_deadband_ccw_);
  read_tuning_param("punch", punch_, has_punch_);
  read_tuning_param("velocity_p_gain", velocity_p_gain_, has_velocity_p_gain_);
  read_tuning_param("velocity_i_gain", velocity_i_gain_, has_velocity_i_gain_);

  auto read_tuning_param_per_servo = [&](const char *base_key, std::vector<int> &values,
                                         std::vector<bool> &has_values)
  {
    for (size_t i = 0; i < sts_joint_count_; ++i)
    {
      std::string key = std::string(base_key) + "_" + std::to_string(i + 1);
      auto it = info_.hardware_parameters.find(key);
      if (it == info_.hardware_parameters.end())
      {
        continue;
      }
      try
      {
        values[i] = std::stoi(it->second);
        has_values[i] = true;
      }
      catch (...)
      {
        RCLCPP_WARN(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                    "Invalid %s value '%s'", key.c_str(), it->second.c_str());
      }
    }
  };

  read_tuning_param_per_servo("position_p_gain", position_p_gain_per_servo_, has_position_p_gain_per_servo_);
  read_tuning_param_per_servo("position_d_gain", position_d_gain_per_servo_, has_position_d_gain_per_servo_);
  read_tuning_param_per_servo("position_i_gain", position_i_gain_per_servo_, has_position_i_gain_per_servo_);
  read_tuning_param_per_servo("deadband_cw", deadband_cw_per_servo_, has_deadband_cw_per_servo_);
  read_tuning_param_per_servo("deadband_ccw", deadband_ccw_per_servo_, has_deadband_ccw_per_servo_);
  read_tuning_param_per_servo("punch", punch_per_servo_, has_punch_per_servo_);
  read_tuning_param_per_servo("velocity_p_gain", velocity_p_gain_per_servo_, has_velocity_p_gain_per_servo_);
  read_tuning_param_per_servo("velocity_i_gain", velocity_i_gain_per_servo_, has_velocity_i_gain_per_servo_);

  position_commands_.reserve(info_.joints.size());    // Reserve memory for the position command. position_commands = the vector of target positions that the controller will write to. 
  position_states_.reserve(info_.joints.size());      // position_states = the vector of current positions that the controller will read from the hardware.
  prev_position_commands_.reserve(info_.joints.size()); // prev_position_commands = a copy of the last commands we sent, used to check if the command has changed before writing to the hardware.

  return CallbackReturn::SUCCESS;
}


// This function answers the question: "What data can controllers READ from this hardware?"
// A StateInterface is a named pointer to a memory location inside the class.

// Example interface object:
// hardware_interface::StateInterface(
//     info_.joints[i].name,           // "joint_1"
//     hardware_interface::HW_IF_POSITION,  // "position"
//     &position_states_[i]            // pointer to the actual double value
// )
// "joint_1 → position → lives at memory address 0x7f3a..."
std::vector<hardware_interface::StateInterface> ManipulatorInterfaceSTS3215::export_state_interfaces()
{
  std::vector<hardware_interface::StateInterface> state_interfaces;

  for (size_t i = 0; i < info_.joints.size(); i++)
  {
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &position_states_[i]));
  }

  return state_interfaces;
}

std::vector<hardware_interface::CommandInterface> ManipulatorInterfaceSTS3215::export_command_interfaces()
{
  std::vector<hardware_interface::CommandInterface> command_interfaces;

  for (size_t i = 0; i < info_.joints.size(); i++)
  {
    command_interfaces.emplace_back(hardware_interface::CommandInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &position_commands_[i]));
  }

  return command_interfaces;
}

CallbackReturn ManipulatorInterfaceSTS3215::on_activate(const rclcpp_lifecycle::State &)
{
  RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterfaceSTS3215"), "Starting STS3215 hardware ...");

  // Initialize the command and state vectors to the correct size, filled with zeros. The size is based on the number of joints defined in the URDF, which is stored in info_.joints.size().
  position_commands_.assign(info_.joints.size(), 0.0);          // position_commands_ = [0.0, 0.0, 0.0, 0.0]
  prev_position_commands_.assign(info_.joints.size(), 0.0);
  position_states_.assign(info_.joints.size(), 0.0);

  if (!openPort())
  {
    RCLCPP_FATAL_STREAM(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                        "Failed to open serial port " << port_);
    return CallbackReturn::FAILURE;
  }

  if (tuning_enable_ && !applyServoTuning())
  {
    RCLCPP_ERROR(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                 "Servo tuning failed; check tuning parameters and wiring.");
    return CallbackReturn::FAILURE;
  }

  pi_ = pigpio_start(nullptr, nullptr);
  if (pi_ < 0)
  {
    RCLCPP_FATAL(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                 "Failed to connect to pigpiod daemon! Error code: %d. Make sure pigpiod is running.", pi_);
    return CallbackReturn::FAILURE;
  }
  gpio_initialized_ = true;
  set_mode(pi_, gpio_gripper_pin_, PI_OUTPUT);

  if (set_servo_pulsewidth(pi_, gpio_gripper_pin_, PWM_CENTER) != 0)
  {
    RCLCPP_FATAL(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                 "Failed to set gripper servo on GPIO pin %d", gpio_gripper_pin_);
    return CallbackReturn::FAILURE;
  }

  return CallbackReturn::SUCCESS;
}

CallbackReturn ManipulatorInterfaceSTS3215::on_deactivate(const rclcpp_lifecycle::State &)
{
  RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterfaceSTS3215"), "Stopping STS3215 hardware ...");
  closePort();

  if (gpio_initialized_)
  {
    set_servo_pulsewidth(pi_, gpio_gripper_pin_, 0);
    pigpio_stop(pi_);
    gpio_initialized_ = false;
  }

  return CallbackReturn::SUCCESS;
}

// “Pull sensor data from hardware → update state variables”
// Hardware = STS3215 servos
// State = position_states_ (joint angles in radians)
hardware_interface::return_type ManipulatorInterfaceSTS3215::read(const rclcpp::Time &,
                                                                  const rclcpp::Duration &)
{
  if (!feedback_from_hardware_ || !serial_open_)
  {
    position_states_ = position_commands_;
    return hardware_interface::return_type::OK;
  }

  bool any_read = false;
  size_t count = std::min(servo_ids_.size(), sts_joint_count_);
  for (size_t i = 0; i < count; ++i)
  {
    uint16_t raw_position = 0;
    if (readPresentPosition(static_cast<uint8_t>(servo_ids_[i]), raw_position))
    {
      position_states_[i] = rawToRadians(raw_position, i);
      any_read = true;
    }
  }

  if (position_states_.size() > sts_joint_count_)
  {
    position_states_[sts_joint_count_] = position_commands_[sts_joint_count_];
  }

  if (!any_read)
  {
    if (!warned_feedback_)
    {
      RCLCPP_WARN(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                  "No feedback received; mirroring commanded positions.");
      warned_feedback_ = true;
    }
    position_states_ = position_commands_;
  }

  return hardware_interface::return_type::OK;
}

hardware_interface::return_type ManipulatorInterfaceSTS3215::write(const rclcpp::Time &,
                                                                   const rclcpp::Duration &)
{
  if (position_commands_ == prev_position_commands_)
  {
    return hardware_interface::return_type::OK;
  }

  if (!serial_open_)
  {
    RCLCPP_ERROR(rclcpp::get_logger("ManipulatorInterfaceSTS3215"), "Serial port is not open");
    return hardware_interface::return_type::ERROR;
  }

  if (!gpio_initialized_)
  {
    RCLCPP_ERROR(rclcpp::get_logger("ManipulatorInterfaceSTS3215"), "GPIO not initialized");
    return hardware_interface::return_type::ERROR;
  }

  size_t count = std::min(servo_ids_.size(), sts_joint_count_);
  for (size_t i = 0; i < count; ++i)
  {
    uint16_t raw = radiansToRaw(position_commands_[i], i);
    if (!writeGoalPosition(static_cast<uint8_t>(servo_ids_[i]), raw))
    {
      RCLCPP_ERROR_STREAM(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                          "Failed to write goal position for servo " << servo_ids_[i]);
      return hardware_interface::return_type::ERROR;
    }
  }

  if (position_commands_.size() > sts_joint_count_)
  {
    int gripper_angle = static_cast<int>(((-position_commands_[sts_joint_count_]) * 180) / (M_PI / 2));
    gripper_angle = std::clamp(gripper_angle, 0, 180);
    int gripper_pulse = angleToPulseWidth(gripper_angle * M_PI / 180.0, 0.0, M_PI);
    set_servo_pulsewidth(pi_, gpio_gripper_pin_, gripper_pulse);
  }

  prev_position_commands_ = position_commands_;
  return hardware_interface::return_type::OK;
}

// This function converts a raw 12-bit encoder value (0–4095) into radians. 
// const at the end — this function promises not to modify any member variables. It's purely a calculation.
double ManipulatorInterfaceSTS3215::rawToRadians(uint16_t raw, size_t joint_index) const
{
  (void)joint_index;
  if (position_raw_max_ <= position_raw_min_ || position_rad_max_ <= position_rad_min_)
  {
    return position_rad_min_;
  }
  double ratio = 0.0;
  if (position_raw_max_ > position_raw_min_)
  {
    ratio = static_cast<double>(raw - position_raw_min_) /
            static_cast<double>(position_raw_max_ - position_raw_min_);
  }

  double radians = position_rad_min_ + ratio * (position_rad_max_ - position_rad_min_);
  if (joint_index == 2)
  {
    radians = -radians;
  }
  return radians;
}

uint16_t ManipulatorInterfaceSTS3215::radiansToRaw(double radians, size_t joint_index) const
{
  if (joint_index == 2)
  {
    radians = -radians;
  }
  if (position_rad_max_ <= position_rad_min_)
  {
    return clampRaw(position_raw_min_);
  }

  double limit_min = position_rad_min_;
  double limit_max = position_rad_max_;

  if (joint_index < info_.joints.size() && !info_.joints[joint_index].command_interfaces.empty())
  {
    const auto &cmd = info_.joints[joint_index].command_interfaces[0];
    double cmd_min = 0.0;
    double cmd_max = 0.0;
    if (parseFiniteDouble(cmd.min, cmd_min) && parseFiniteDouble(cmd.max, cmd_max) && cmd_min < cmd_max)
    {
      limit_min = std::max(cmd_min, position_rad_min_);
      limit_max = std::min(cmd_max, position_rad_max_);
      if (limit_min >= limit_max)
      {
        limit_min = position_rad_min_;
        limit_max = position_rad_max_;
      }
    }
  }

  double clamped = clampRad(radians, limit_min, limit_max);
  double ratio = (clamped - position_rad_min_) / (position_rad_max_ - position_rad_min_);
  int raw = static_cast<int>(std::lround(position_raw_min_ +
                                         ratio * (position_raw_max_ - position_raw_min_)));
  return clampRaw(raw);
}

// const std::string &csv_ids = "a read-only reference to a std::string, called csv_ids." No copy is made, the original string is used directly, and the function is not allowed to modify it(const).
//size_t is an unsigned integer type specifically for sizes and counts 
void ManipulatorInterfaceSTS3215::loadServoIds(const std::string &csv_ids, size_t joint_count)
{
  servo_ids_.clear();

  if (csv_ids.empty())
  {
    for (size_t i = 0; i < joint_count; ++i)
    {
      servo_ids_.push_back(static_cast<int>(i + 1));
      // static_cast<int> converts size_t (unsigned) to int (signed). push_back expects an int to match the std::vector<int> type. 
    }
    return;
  }

  std::stringstream ss(csv_ids);
  std::string token;
  while (std::getline(ss, token, ','))
  {
    try
    {
      servo_ids_.push_back(std::stoi(token));
    }
    catch (...)
    {
      RCLCPP_WARN_STREAM(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                         "Invalid servo id: " << token);
    }
  }

  if (servo_ids_.size() < joint_count)
  {
    for (size_t i = servo_ids_.size(); i < joint_count; ++i)
    {
      servo_ids_.push_back(static_cast<int>(i + 1));
    }
  }
}

// It takes a human-readable number (like 115200) and converts it to a system constant (like B115200) that the Linux serial port API understands.
bool ManipulatorInterfaceSTS3215::openPort()
{
  // // begin() returns true on success, false on failure. It also initializes the serial communication with the specified baudrate and port.
  if (!scs_.begin(baudrate_, port_.c_str()))   // port_.c_str() converts the std::string port_ into a C-style string (const char*) which is what the begin() function expects.
  {
    return false;
  }

  serial_open_ = true;
  return true;
}

void ManipulatorInterfaceSTS3215::closePort()
{
  scs_.end();
  serial_open_ = false;
}

bool ManipulatorInterfaceSTS3215::readPresentPosition(uint8_t servo_id, uint16_t &out_position)
{
  // ReadPos() return either -1 (error) or a value between 0–4095. 
  int position = scs_.ReadPos(servo_id);
  if (position < 0)   // If anything goes wrong (timeout, checksum fail, servo offline), it returns -1.
  {
    return false;
  }

  out_position = clampRaw(position);   // out_position is passed by reference, so we modify the caller's variable directly. We also clamp it to ensure it's within valid bounds.
  return true;
}

bool ManipulatorInterfaceSTS3215::writeGoalPosition(uint8_t servo_id, uint16_t position)
{
  return scs_.WritePosEx(servo_id, static_cast<int>(position), servo_speed_, servo_acceleration_) == 1;
         //               │          │                         │  │
         //               ID         target position       speed  acceleration
}

bool ManipulatorInterfaceSTS3215::applyServoTuning()
{
  if (!serial_open_)
  {
    return false;
  }

  bool ok = true;
  size_t count = std::min(servo_ids_.size(), sts_joint_count_);
  for (size_t i = 0; i < count; ++i)
  {
    uint8_t id = static_cast<uint8_t>(servo_ids_[i]);
    bool servo_ok = true;
    if (!scs_.unLockEeprom(id))
    {
      RCLCPP_WARN(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                  "Failed to unlock EEPROM for servo %u", id);
      servo_ok = false;
      ok = false;
      continue;
    }

    auto write_byte = [&](uint8_t addr, int value)
    {
      uint8_t clamped = static_cast<uint8_t>(std::clamp(value, 0, 255));
      if (!scs_.writeByte(id, addr, clamped))
      {
        RCLCPP_WARN(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                    "Failed to write addr %u for servo %u", addr, id);
        servo_ok = false;
        ok = false;
      }
      else
      {
        RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                    "Wrote addr %u value %u to servo %u", addr, clamped, id);
      }
    };

    auto write_word = [&](uint8_t addr, int value)
    {
      uint16_t clamped = static_cast<uint16_t>(std::clamp(value, 0, 65535));
      if (!scs_.writeWord(id, addr, clamped))
      {
        RCLCPP_WARN(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                    "Failed to write word addr %u for servo %u", addr, id);
        servo_ok = false;
        ok = false;
      }
      else
      {
        RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                    "Wrote word addr %u value %u to servo %u", addr, clamped, id);
      }
    };

    auto get_value = [&](const std::vector<bool> &has_values, const std::vector<int> &values,
                         bool has_global, int global_value, int &out_value) -> bool
    {
      if (i < has_values.size() && has_values[i])
      {
        out_value = values[i];
        return true;
      }
      if (has_global)
      {
        out_value = global_value;
        return true;
      }
      return false;
    };

    int value = 0;
    bool has_p = get_value(has_position_p_gain_per_servo_, position_p_gain_per_servo_,
                           has_position_p_gain_, position_p_gain_, value);
    int p_value = value;
    if (has_p)
    {
      write_byte(STS_POS_P_GAIN_ADDR, value);
    }

    bool has_d = get_value(has_position_d_gain_per_servo_, position_d_gain_per_servo_,
                           has_position_d_gain_, position_d_gain_, value);
    int d_value = value;
    if (has_d)
    {
      write_byte(STS_POS_D_GAIN_ADDR, value);
    }

    bool has_i = get_value(has_position_i_gain_per_servo_, position_i_gain_per_servo_,
                           has_position_i_gain_, position_i_gain_, value);
    int i_value = value;
    if (has_i)
    {
      write_byte(STS_POS_I_GAIN_ADDR, value);
    }

    bool has_punch = get_value(has_punch_per_servo_, punch_per_servo_,
                               has_punch_, punch_, value);
    int punch_value = value;
    if (has_punch)
    {
      write_byte(STS_PUNCH_ADDR, value);
    }

    bool has_cw = get_value(has_deadband_cw_per_servo_, deadband_cw_per_servo_,
                            has_deadband_cw_, deadband_cw_, value);
    int cw_value = value;
    if (has_cw)
    {
      write_byte(STS_CW_DEADBAND_ADDR, value);
    }

    bool has_ccw = get_value(has_deadband_ccw_per_servo_, deadband_ccw_per_servo_,
                             has_deadband_ccw_, deadband_ccw_, value);
    int ccw_value = value;
    if (has_ccw)
    {
      write_byte(STS_CCW_DEADBAND_ADDR, value);
    }

    bool has_vel_p = get_value(has_velocity_p_gain_per_servo_, velocity_p_gain_per_servo_,
                               has_velocity_p_gain_, velocity_p_gain_, value);
    int vel_p_value = value;
    if (has_vel_p)
    {
      write_word(STS_VEL_P_GAIN_ADDR, value);
    }

    bool has_vel_i = get_value(has_velocity_i_gain_per_servo_, velocity_i_gain_per_servo_,
                               has_velocity_i_gain_, velocity_i_gain_, value);
    int vel_i_value = value;
    if (has_vel_i)
    {
      write_word(STS_VEL_I_GAIN_ADDR, value);
    }

    if (has_p || has_d || has_i || has_punch || has_cw || has_ccw || has_vel_p || has_vel_i)
    {
      std::ostringstream log_line;
      log_line << "Servo " << static_cast<unsigned>(id) << " tuning: ";
      if (has_p)
      {
        log_line << "P=" << p_value << " ";
      }
      if (has_d)
      {
        log_line << "D=" << d_value << " ";
      }
      if (has_i)
      {
        log_line << "I=" << i_value << " ";
      }
      if (has_cw)
      {
        log_line << "CW=" << cw_value << " ";
      }
      if (has_ccw)
      {
        log_line << "CCW=" << ccw_value << " ";
      }
      if (has_punch)
      {
        log_line << "Punch=" << punch_value << " ";
      }
      if (has_vel_p)
      {
        log_line << "VelP=" << vel_p_value << " ";
      }
      if (has_vel_i)
      {
        log_line << "VelI=" << vel_i_value;
      }
      RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterfaceSTS3215"), "%s", log_line.str().c_str());
    }

    if (!scs_.LockEeprom(id))
    {
      RCLCPP_WARN(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                  "Failed to lock EEPROM for servo %u", id);
      servo_ok = false;
      ok = false;
    }

    if (servo_ok)
    {
      RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                  "Servo %u tuning write succeeded.", id);
    }
  }

  if (ok)
  {
    RCLCPP_INFO(rclcpp::get_logger("ManipulatorInterfaceSTS3215"),
                "Servo tuning parameters written successfully.");
  }

  return ok;
}

uint16_t ManipulatorInterfaceSTS3215::clampRaw(int value) const
{
  if (value < static_cast<int>(position_raw_min_))
  {
    return position_raw_min_;
  }
  if (value > static_cast<int>(position_raw_max_))
  {
    return position_raw_max_;
  }
  return static_cast<uint16_t>(value);
}

double ManipulatorInterfaceSTS3215::clampRad(double value, double min_val, double max_val) const
{
  if (value < min_val)
  {
    return min_val;
  }
  if (value > max_val)
  {
    return max_val;
  }
  return value;
}

int ManipulatorInterfaceSTS3215::angleToPulseWidth(double angle, double min_angle, double max_angle)
{
  double normalized = (angle - min_angle) / (max_angle - min_angle);
  normalized = std::clamp(normalized, 0.0, 1.0);

  int pulse_width = PWM_MIN + static_cast<int>(normalized * (PWM_MAX - PWM_MIN));
  return std::clamp(pulse_width, PWM_MIN, PWM_MAX);
}

}  // namespace manipulator_controller

PLUGINLIB_EXPORT_CLASS(manipulator_controller::ManipulatorInterfaceSTS3215, hardware_interface::SystemInterface)
