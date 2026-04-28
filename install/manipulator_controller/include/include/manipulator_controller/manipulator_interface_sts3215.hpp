#ifndef MANIPULATOR_INTERFACE_STS3215_H
#define MANIPULATOR_INTERFACE_STS3215_H
//This is called an include guard. Headers can accidentally get included multiple times. This guard prevents that.

#include <rclcpp/rclcpp.hpp>
#include <hardware_interface/system_interface.hpp>  //the base class this code builds on
#include <rclcpp_lifecycle/state.hpp>
#include <rclcpp_lifecycle/node_interfaces/lifecycle_node_interface.hpp> //handles the lifecycle state machine (init → activate → deactivate)
#include <cstdint>     // for fixed-width integer types like uint8_t, uint16_t
#include <cstddef>
#include <cmath>

#include <string>
#include <vector>

#include "SMS_STS.h"   // The library for communicating with the STS3215 servos. 

namespace manipulator_controller  // The namespace for this code, to avoid name conflicts with other code
{

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn; 
// That full type name is painfully long. This line creates a short alias so you can write CallbackReturn everywhere instead of the full thing.

// SystemInterface is the parent (base) class, defined by the ROS 2 framework.
// ManipulatorInterfaceSTS3215 is the child class — it gets everything the parent has, and adds or overrides specific parts
// The public keyword means the parent's public members stay public in the child
class ManipulatorInterfaceSTS3215 : public hardware_interface::SystemInterface 
{
public:  // these can be called by anyone
  ManipulatorInterfaceSTS3215();
  virtual ~ManipulatorInterfaceSTS3215();

  virtual CallbackReturn on_activate(const rclcpp_lifecycle::State &previous_state) override;       // called when the robot is enabled — opens serial port
  virtual CallbackReturn on_deactivate(const rclcpp_lifecycle::State &previous_state) override;     // called when the robot is disabled — closes serial port
  virtual CallbackReturn on_init(const hardware_interface::HardwareInfo &hardware_info) override;   // called once at startup — reads config from URDF

  virtual std::vector<hardware_interface::StateInterface> export_state_interfaces() override;       // State interfaces = things you can read (joint positions).
  virtual std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;   // Command interfaces = things you can write (target positions).

  // These are called every control cycle (typically 100–1000 Hz). read() pulls current positions from the servos; write() sends new target positions.
  virtual hardware_interface::return_type read(const rclcpp::Time &time, const rclcpp::Duration &period) override;
  virtual hardware_interface::return_type write(const rclcpp::Time &time, const rclcpp::Duration &period) override;

private:  // these can only be used inside the class itself
  void loadServoIds(const std::string &csv_ids, size_t joint_count);
  bool openPort();                  // Opens the serial port to communicate with the servos. Called in on_activate().                                                                   
  void closePort();                 // Closes the serial port. Called in on_deactivate() and the destructor.
  bool readPresentPosition(uint8_t servo_id, uint16_t &out_position);   // Reads the current position of a servo by its ID. Returns true on success and fills out_position with the raw encoder value.
  bool writeGoalPosition(uint8_t servo_id, uint16_t position);          // Writes a target position to a servo by its ID. Returns true on success.
  bool applyServoTuning();
  double rawToRadians(uint16_t raw, size_t joint_index) const;          // Converts a raw encoder value from the servo into radians using the servo's global range.
  uint16_t radiansToRaw(double radians, size_t joint_index) const;      // Converts a target position in radians into a raw encoder value using the servo's global range and joint limits for clamping.
  uint16_t clampRaw(int value) const;                                   // Clamps a raw encoder value to the valid range defined by position_raw_min_ and position_raw_max_.
  double clampRad(double value, double min_val, double max_val) const;  // Clamps a radian value to the valid range defined by min_val and max_val, which are typically the joint limits from the URDF.

  bool serial_open_;                // Whether the serial port is currently open.
  std::string port_;                // The name of the serial port to use, e.g. "/dev/ttyUSB0". Loaded from URDF.
  int baudrate_;                    // The baudrate for the serial communication, e.g. 1000000. Loaded from URDF.
  bool feedback_from_hardware_;     // Whether to read actual positions from the hardware (true) or just mirror the commanded positions (false). Loaded from URDF.
  bool warned_feedback_;            // Whether we've already warned about missing feedback, to avoid spamming the logs.

  SMS_STS scs_;   // The object from the SMS_STS library that handles the low-level communication with the servos.

  uint8_t goal_position_addr_;      // The register address to write target positions to, e.g. 0x2A. Loaded from URDF.
  uint8_t present_position_addr_;   // The register address to read current positions from, e.g. 0x38. Loaded from URDF.
  uint16_t position_raw_min_;       // The minimum raw encoder value corresponding to the joint's minimum position. Loaded from URDF.
  uint16_t position_raw_max_;       // The maximum raw encoder value corresponding to the joint's maximum position. Loaded from URDF.
  double position_rad_min_;         // The minimum servo angle (radians) corresponding to position_raw_min_. Loaded from URDF.
  double position_rad_max_;         // The maximum servo angle (radians) corresponding to position_raw_max_. Loaded from URDF.
  uint16_t servo_speed_;            // Default move speed for WritePosEx (0 = use servo default).
  uint8_t servo_acceleration_;      // Default acceleration for WritePosEx (0 = use servo default).

  bool tuning_enable_;
  bool has_position_p_gain_;
  bool has_position_d_gain_;
  bool has_position_i_gain_;
  bool has_deadband_cw_;
  bool has_deadband_ccw_;
  bool has_punch_;
  int position_p_gain_;
  int position_d_gain_;
  int position_i_gain_;
  int deadband_cw_;
  int deadband_ccw_;
  int punch_;
  std::vector<bool> has_position_p_gain_per_servo_;
  std::vector<bool> has_position_d_gain_per_servo_;
  std::vector<bool> has_position_i_gain_per_servo_;
  std::vector<bool> has_deadband_cw_per_servo_;
  std::vector<bool> has_deadband_ccw_per_servo_;
  std::vector<bool> has_punch_per_servo_;
  std::vector<int> position_p_gain_per_servo_;
  std::vector<int> position_d_gain_per_servo_;
  std::vector<int> position_i_gain_per_servo_;
  std::vector<int> deadband_cw_per_servo_;
  std::vector<int> deadband_ccw_per_servo_;
  std::vector<int> punch_per_servo_;

  bool has_velocity_p_gain_;
  bool has_velocity_i_gain_;
  int velocity_p_gain_;
  int velocity_i_gain_;
  std::vector<bool> has_velocity_p_gain_per_servo_;
  std::vector<bool> has_velocity_i_gain_per_servo_;
  std::vector<int> velocity_p_gain_per_servo_;
  std::vector<int> velocity_i_gain_per_servo_;

  std::vector<int> servo_ids_;      // The list of servo IDs to control, e.g. [1, 2, 3, 4]. Loaded from URDF.
  std::vector<double> position_commands_;       // The target positions for each joint, in radians. This is what the controller writes to, and write() sends these to the servos.
  std::vector<double> prev_position_commands_;  // The target positions from the previous control cycle, used to check if we need to send new commands.
  std::vector<double> position_states_;         // The current positions of each joint, in radians, as read from the servos. This is what the controller reads from, and read() updates these values based on feedback from the hardware (or mirrors the commands if feedback is disabled).

  // Consecutive read-failure guard.
  // Prevents position_states_ from alternating between real servo position and
  // commanded position on intermittent ReadPos failures, which caused large
  // output spikes in the JointTrajectoryController.
  // Strategy:
  //   success          → reset counter, store last_known_position_, update state.
  //   fail < threshold → hold last_known_position_ (no flip, no spike).
  //   fail >= threshold→ mirror position_commands_ and warn once.
  std::vector<int>    consecutive_read_failures_; // per-servo failure counter
  std::vector<double> last_known_position_;        // last successful read per servo, radians
  static constexpr int READ_FAIL_THRESHOLD = 3;   // tolerate up to 3 consecutive missed reads

  size_t sts_joint_count_;

  static constexpr uint8_t STS_POS_P_GAIN_ADDR = 21;
  static constexpr uint8_t STS_POS_D_GAIN_ADDR = 22;
  static constexpr uint8_t STS_POS_I_GAIN_ADDR = 23;
  static constexpr uint8_t STS_PUNCH_ADDR = 24;
  static constexpr uint8_t STS_CW_DEADBAND_ADDR = 26;
  static constexpr uint8_t STS_CCW_DEADBAND_ADDR = 27;
  static constexpr uint8_t STS_VEL_P_GAIN_ADDR = 37;  // 2-byte EEPROM register
  static constexpr uint8_t STS_VEL_I_GAIN_ADDR = 39;  // 2-byte EEPROM register
};

}  // namespace manipulator_controller

#endif  // MANIPULATOR_INTERFACE_STS3215_H