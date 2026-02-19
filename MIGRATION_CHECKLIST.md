# Migration Checklist: Arduino → Raspberry Pi GPIO

## ✅ Pre-Migration (Preparation)

- [ ] Review the [README_RPI_GPIO.md](README_RPI_GPIO.md) documentation
- [ ] Ensure you have all required hardware:
  - [ ] Raspberry Pi (any model with GPIO)
  - [ ] 4x Servo motors
  - [ ] External 5-6V power supply (2A minimum)
  - [ ] Jumper wires
- [ ] Backup your current system configuration

## 🔧 Software Installation

- [ ] Run the setup script:
  ```bash
  sudo ./setup_rpi_gpio.sh
  ```
- [ ] Verify pigpiod is running:
  ```bash
  sudo systemctl status pigpiod
  ```
- [ ] Rebuild the workspace:
  ```bash
  colcon build
  source install/setup.bash
  ```

## 🔌 Hardware Wiring

⚠️ **IMPORTANT**: Disconnect power before wiring!

### Step 1: Remove Arduino
- [ ] Power down the entire system
- [ ] Disconnect Arduino USB cable
- [ ] Disconnect servo wires from Arduino
- [ ] (Optional) Remove Arduino board completely

### Step 2: Prepare Power Supply
- [ ] Connect external 5-6V power supply ground to breadboard/common rail
- [ ] Connect Raspberry Pi GND to the common ground rail
- [ ] DO NOT connect servo VCC to Raspberry Pi 5V pins!

### Step 3: Connect Servos to GPIO

Base Servo:
- [ ] Signal wire (orange/yellow) → GPIO 17 (Physical Pin 11)
- [ ] Power wire (red) → External PSU 5-6V+
- [ ] Ground wire (brown/black) → Common ground

Shoulder Servo:
- [ ] Signal wire → GPIO 18 (Physical Pin 12)
- [ ] Power wire → External PSU 5-6V+
- [ ] Ground wire → Common ground

Elbow Servo:
- [ ] Signal wire → GPIO 27 (Physical Pin 13)
- [ ] Power wire → External PSU 5-6V+
- [ ] Ground wire → Common ground

Gripper Servo:
- [ ] Signal wire → GPIO 22 (Physical Pin 15)
- [ ] Power wire → External PSU 5-6V+
- [ ] Ground wire → Common ground

### Step 4: Verify Connections
- [ ] Double-check all signal wires are on correct GPIO pins
- [ ] Verify common ground connection (Pi GND ↔ Servo GND ↔ PSU GND)
- [ ] Ensure no servo is powered from Pi 5V pins
- [ ] Check for any loose connections

## 🧪 Testing

### Step 1: Power On Safety Check
- [ ] Power on external servo power supply
- [ ] Verify voltage is 5-6V (use multimeter if available)
- [ ] Power on Raspberry Pi
- [ ] Check that no components are getting hot

### Step 2: Manual Servo Test
- [ ] Run the test script:
  ```bash
  ./test_gpio_servos.sh
  ```
- [ ] Verify each servo responds correctly
- [ ] Note any servos that don't move (check wiring)

### Step 3: ROS2 Integration Test
- [ ] Launch the controller:
  ```bash
  ros2 launch manipulator_controller controller.launch.py is_sim:=false
  ```
- [ ] Check for any errors in the output
- [ ] Verify all joints are detected
- [ ] Test with a simple trajectory

### Step 4: Full System Test
- [ ] Launch with RViz:
  ```bash
  ros2 launch manipulator_moveit demo.launch.py is_sim:=false
  ```
- [ ] Plan and execute a simple motion
- [ ] Verify smooth servo movement
- [ ] Test all 4 joints individually

## 🐛 Troubleshooting

If you encounter issues, check these:

- [ ] **No servo movement**:
  - [ ] pigpiod running? `sudo systemctl status pigpiod`
  - [ ] Correct GPIO pins in URDF?
  - [ ] External power supply connected and on?
  - [ ] Common ground connected?

- [ ] **Erratic servo movement**:
  - [ ] Power supply adequate (2A+)?
  - [ ] Signal wires not too long?
  - [ ] Add capacitor (470μF) across servo power?

- [ ] **Some servos work, others don't**:
  - [ ] Check individual servo wiring
  - [ ] Test with `pigs s <gpio> 1500`
  - [ ] Verify GPIO pin numbers in URDF match physical connections

- [ ] **ROS2 errors**:
  - [ ] Rebuild: `colcon build`
  - [ ] Source: `source install/setup.bash`
  - [ ] Check logs: `ros2 launch ... --show-messages`

## ✨ Post-Migration

- [ ] Document your GPIO pin configuration (if different from defaults)
- [ ] Test all robot capabilities:
  - [ ] Manual control
  - [ ] Trajectory execution
  - [ ] MoveIt planning
  - [ ] Gripper operation
- [ ] Create a system backup
- [ ] Update any custom launch files if needed
- [ ] (Optional) Remove Arduino-related packages if not needed:
  ```bash
  # You can remove manipulator_arduino_firmware if desired
  rm -rf src/manipulator_arduino_firmware
  ```

## 📝 Configuration Reference

Default GPIO pins (change in URDF if needed):
```
Base:     GPIO 17 (Pin 11)
Shoulder: GPIO 18 (Pin 12)
Elbow:    GPIO 27 (Pin 13)
Gripper:  GPIO 22 (Pin 15)
```

To change pins, edit:
`src/manipulator_description/urdf/manipulator_ros2_control.xacro`

## 🎯 Success Criteria

Your migration is successful when:
- [x] All servos respond to GPIO commands
- [x] ROS2 controller launches without errors
- [x] Robot can execute planned trajectories
- [x] Movement is smooth and accurate
- [x] No hardware getting hot or unstable

## 📚 Additional Resources

- [README_RPI_GPIO.md](README_RPI_GPIO.md) - Detailed documentation
- [setup_rpi_gpio.sh](setup_rpi_gpio.sh) - Installation script
- [test_gpio_servos.sh](test_gpio_servos.sh) - Test utility
- [pigpio documentation](http://abyz.me.uk/rpi/pigpio/)

---

**Need Help?**
- Review the README_RPI_GPIO.md troubleshooting section
- Check ROS2 logs for specific error messages
- Verify hardware connections with multimeter
- Test servos individually with `pigs` commands
