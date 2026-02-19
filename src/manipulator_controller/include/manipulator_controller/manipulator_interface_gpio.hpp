#ifndef MANIPULATOR_INTERFACE_GPIO_H
#define MANIPULATOR_INTERFACE_GPIO_H

#include <rclcpp/rclcpp.hpp>
#include <hardware_interface/system_interface.hpp>
#include <rclcpp_lifecycle/state.hpp>
#include <rclcpp_lifecycle/node_interfaces/lifecycle_node_interface.hpp>
#include <pigpio.h>

#include <vector>
#include <string>
#include <array>


namespace manipulator_controller
{

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

/**
 * @brief Hardware interface for controlling the manipulator using Raspberry Pi GPIO
 * 
 * This class replaces the Arduino serial communication with direct GPIO PWM control
 * for servo motors using the pigpio library on Raspberry Pi.
 */
class ManipulatorInterfaceGPIO : public hardware_interface::SystemInterface
{
public:
  ManipulatorInterfaceGPIO();
  virtual ~ManipulatorInterfaceGPIO();

  // Implementing rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface
  virtual CallbackReturn on_activate(const rclcpp_lifecycle::State &previous_state) override;
  virtual CallbackReturn on_deactivate(const rclcpp_lifecycle::State &previous_state) override;

  // Implementing hardware_interface::SystemInterface
  virtual CallbackReturn on_init(const hardware_interface::HardwareInfo &hardware_info) override;
  virtual std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
  virtual std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;
  virtual hardware_interface::return_type read(const rclcpp::Time & time, const rclcpp::Duration & period) override;
  virtual hardware_interface::return_type write(const rclcpp::Time & time, const rclcpp::Duration & period) override;

private:
  /**
   * @brief Convert angle in radians to PWM pulse width in microseconds
   * @param angle Angle in radians
   * @param min_angle Minimum angle in radians
   * @param max_angle Maximum angle in radians
   * @return PWM pulse width in microseconds (typically 500-2500us for servos)
   */
  int angleToPulseWidth(double angle, double min_angle = -M_PI/2, double max_angle = M_PI/2);

  /**
   * @brief Set servo position using GPIO PWM
   * @param gpio_pin GPIO pin number
   * @param pulse_width PWM pulse width in microseconds
   */
  void setServoPosition(int gpio_pin, int pulse_width);

  // GPIO pin assignments for each servo
  std::array<int, 4> gpio_pins_;  // [base, shoulder, elbow, gripper]
  
  // PWM parameters optimized for MG995 servos (microseconds)
  static constexpr int PWM_MIN = 1000;  // Minimum pulse width (0 degrees) - MG995 optimal
  static constexpr int PWM_MAX = 2000;  // Maximum pulse width (180 degrees) - MG995 optimal
  static constexpr int PWM_CENTER = 1500; // Center position (90 degrees)
  static constexpr int PWM_FREQ = 50;   // Standard servo frequency (50Hz = 20ms period)
  
  // Joint state and command vectors
  std::vector<double> position_commands_;
  std::vector<double> prev_position_commands_;
  std::vector<double> position_states_;
  
  // GPIO initialization flag
  bool gpio_initialized_;
};

}  // namespace manipulator_controller

#endif  // MANIPULATOR_INTERFACE_GPIO_H
