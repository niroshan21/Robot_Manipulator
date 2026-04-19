#ifndef MANIPULATOR_INTERFACE_STS3215_H
#define MANIPULATOR_INTERFACE_STS3215_H

#include <rclcpp/rclcpp.hpp>
#include <hardware_interface/system_interface.hpp>
#include <rclcpp_lifecycle/state.hpp>
#include <rclcpp_lifecycle/node_interfaces/lifecycle_node_interface.hpp>
#include <libserial/SerialPort.h>

#include <string>
#include <vector>

namespace manipulator_controller
{

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class ManipulatorInterfaceSTS3215 : public hardware_interface::SystemInterface
{
public:
  ManipulatorInterfaceSTS3215();
  virtual ~ManipulatorInterfaceSTS3215();

  virtual CallbackReturn on_activate(const rclcpp_lifecycle::State &previous_state) override;
  virtual CallbackReturn on_deactivate(const rclcpp_lifecycle::State &previous_state) override;

  virtual CallbackReturn on_init(const hardware_interface::HardwareInfo &hardware_info) override;
  virtual std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
  virtual std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;
  virtual hardware_interface::return_type read(const rclcpp::Time &time, const rclcpp::Duration &period) override;
  virtual hardware_interface::return_type write(const rclcpp::Time &time, const rclcpp::Duration &period) override;

private:
  int radiansToServoDegrees(size_t joint_index, double radians) const;
  void loadServoIds(const std::string &csv_ids, size_t joint_count);
  LibSerial::BaudRate resolveBaudRate(int baudrate) const;

  LibSerial::SerialPort serial_;
  std::string port_;
  int baudrate_;
  bool feedback_from_hardware_;
  bool warned_feedback_;
  bool warned_protocol_;

  std::vector<int> servo_ids_;
  std::vector<double> position_commands_;
  std::vector<double> prev_position_commands_;
  std::vector<double> position_states_;
};

}  // namespace manipulator_controller

#endif  // MANIPULATOR_INTERFACE_STS3215_H
