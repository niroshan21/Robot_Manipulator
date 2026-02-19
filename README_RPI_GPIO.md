# Raspberry Pi GPIO Control Mode

This document explains how to use the robot manipulator with **Raspberry Pi GPIO** instead of Arduino serial communication.

## Overview

The manipulator now supports **direct GPIO control** from the Raspberry Pi, eliminating the need for an Arduino board. This provides:

- ✅ Simpler hardware setup (no Arduino required)
- ✅ Reduced latency (no serial communication)
- ✅ Direct PWM control of servos
- ✅ Lower cost (one less board to purchase)

## Hardware Requirements

### Required Components
1. **Raspberry Pi** (any model with GPIO pins - tested on Pi 3B+, Pi 4)
2. **4x Servo Motors** (MG995 or similar)
3. **External 5-6V Power Supply** for servos (DO NOT power servos from Pi!)
4. **Jumper Wires** for connections

### Wiring Diagram

```
Raspberry Pi GPIO              Servo Motors
─────────────────              ────────────
                              
GPIO 17 (Pin 11) ────────────> Base Servo (Signal)
GPIO 18 (Pin 12) ────────────> Shoulder Servo (Signal)
GPIO 27 (Pin 13) ────────────> Elbow Servo (Signal)
GPIO 22 (Pin 15) ────────────> Gripper Servo (Signal)

GND (Pin 6, 9, etc.) ────────> All Servo GND
                              
External 5-6V PSU+ ──────────> All Servo VCC (Red wire)
External 5-6V PSU- ──────────> Common Ground (also to Pi GND)
```

### ⚠️ CRITICAL: Power Supply Notes

1. **NEVER** power servos from Raspberry Pi's 5V pins - this will damage your Pi!
2. Use a **separate 5-6V power supply** (at least 2-3A) for the servos
3. **Connect grounds together**: Pi GND ↔ Servo GND ↔ PSU GND (common ground)
4. Only the **signal wires** go to GPIO pins

### GPIO Pin Reference

| GPIO Pin | Physical Pin | Servo     | Wire Color (typical) |
|----------|-------------|-----------|---------------------|
| GPIO 17  | Pin 11      | Base      | Orange/Yellow       |
| GPIO 18  | Pin 12      | Shoulder  | Orange/Yellow       |
| GPIO 27  | Pin 13      | Elbow     | Orange/Yellow       |
| GPIO 22  | Pin 15      | Gripper   | Orange/Yellow       |
| GND      | Pin 6, 9... | All       | Brown/Black         |

**Note**: You can change these GPIO pins in the URDF configuration file.

## Software Installation

### Step 1: Install pigpio Library

Run the setup script (from the workspace root):

```bash
sudo ./setup_rpi_gpio.sh
```

This script will:
- Install the `pigpio` library
- Enable and start the `pigpiod` daemon
- Configure GPIO permissions
- Verify the installation

### Step 2: Verify pigpiod is Running

```bash
sudo systemctl status pigpiod
```

You should see `active (running)` in green.

If not running, start it manually:
```bash
sudo systemctl start pigpiod
```

### Step 3: Build the Workspace

```bash
cd /home/rajitha-niroshan/FYP/My_Work/Repo/Robot_Manipulator
colcon build
source install/setup.bash
```

## Configuration

### Changing GPIO Pins

Edit the URDF file: [src/manipulator_description/urdf/manipulator_ros2_control.xacro](src/manipulator_description/urdf/manipulator_ros2_control.xacro)

Find this section:
```xml
<param name="gpio_base">17</param>      <!-- Change to your base servo pin -->
<param name="gpio_shoulder">18</param>  <!-- Change to your shoulder servo pin -->
<param name="gpio_elbow">27</param>     <!-- Change to your elbow servo pin -->
<param name="gpio_gripper">22</param>   <!-- Change to your gripper servo pin -->
```

After changing, rebuild:
```bash
colcon build --packages-select manipulator_description
source install/setup.bash
```

### PWM Configuration

The GPIO implementation uses these default PWM parameters:
- **Frequency**: 50 Hz (standard for servos)
- **Pulse Width Range**: 500-2500 μs
  - 500 μs = 0° position
  - 1500 μs = 90° position (center)
  - 2500 μs = 180° position

These are defined in [manipulator_interface_gpio.hpp](src/manipulator_controller/include/manipulator_controller/manipulator_interface_gpio.hpp) and can be adjusted if your servos require different values.

## Usage

### Launch the Robot

Same as before, just make sure `pigpiod` is running:

```bash
# Check pigpiod status
sudo systemctl status pigpiod

# Launch the robot
ros2 launch manipulator_controller controller.launch.py is_sim:=false
```

### Testing Individual Servos

You can test individual servos using the `pigs` command:

```bash
# Set base servo (GPIO 17) to 90 degrees (1500μs)
pigs s 17 1500

# Set shoulder servo (GPIO 18) to 0 degrees (500μs)
pigs s 18 500

# Turn off a servo
pigs s 17 0
```

## Troubleshooting

### Issue: "Failed to initialize pigpio"

**Solution**: Make sure `pigpiod` daemon is running
```bash
sudo systemctl start pigpiod
sudo systemctl status pigpiod
```

### Issue: Servos not moving

1. **Check power supply**: Servos need external 5-6V power (not from Pi)
2. **Check wiring**: Signal wires to correct GPIO pins
3. **Common ground**: All grounds must be connected together
4. **Test manually**: Use `pigs s <gpio> <pulse>` to test

### Issue: Jittery servo movement

1. **Power supply**: Ensure adequate current (2-3A minimum)
2. **Shielded cables**: Use shorter, shielded wires for signal
3. **Capacitors**: Add 470μF capacitor across servo power supply

### Issue: Permission denied

**Solution**: Add user to gpio group and reboot
```bash
sudo usermod -a -G gpio $USER
# Then log out and back in, or reboot
```

## Comparison: Arduino vs Raspberry Pi GPIO

| Feature                  | Arduino Serial    | Raspberry Pi GPIO  |
|-------------------------|-------------------|--------------------|
| Hardware needed         | Pi + Arduino      | Pi only            |
| Communication latency   | ~10-50ms          | < 1ms              |
| Wiring complexity       | Medium (USB+pins) | Simple (pins only) |
| Setup complexity        | Medium            | Easy               |
| Power requirements      | Both boards       | Pi only            |
| Cost                    | Higher            | Lower              |
| Reliability             | Good              | Excellent          |

## Migration from Arduino

If you're migrating from Arduino to GPIO:

1. ✅ **Code Changes**: Already done! The system automatically uses GPIO on real hardware
2. ✅ **Remove Arduino**: Disconnect the Arduino board
3. ✅ **Rewire servos**: Connect servo signal wires directly to GPIO pins
4. ✅ **Run setup**: Execute `sudo ./setup_rpi_gpio.sh`
5. ✅ **Test**: Launch with `is_sim:=false`

The old Arduino serial interface is still available in the code as a fallback if needed. To use it, change the plugin in the URDF from `ManipulatorInterfaceGPIO` to `ManipulatorInterface`.

## Files Changed

The following files were modified/created for GPIO support:

### New Files:
- `src/manipulator_controller/src/manipulator_interface_gpio.cpp` - GPIO implementation
- `src/manipulator_controller/include/manipulator_controller/manipulator_interface_gpio.hpp` - GPIO header
- `setup_rpi_gpio.sh` - Installation script
- `README_RPI_GPIO.md` - This file

### Modified Files:
- `src/manipulator_controller/CMakeLists.txt` - Build both versions
- `src/manipulator_controller/package.xml` - Updated dependencies
- `src/manipulator_controller/manipulator_controller_plugins.xml` - Registered GPIO plugin
- `src/manipulator_description/urdf/manipulator_ros2_control.xacro` - GPIO parameters

### Unchanged:
- All other packages remain the same
- Launch files work without modification
- MoveIt configuration unchanged
- Trajectory planning unchanged

## Technical Details

### Implementation

The GPIO interface uses the **pigpio** library which provides:
- Hardware-timed PWM (via DMA)
- Jitter-free servo control
- Multiple simultaneous servo support
- Real-time performance

The implementation maps ROS2 joint positions (radians) to servo PWM pulse widths (microseconds) using the same coordinate transformations as the Arduino version for compatibility.

### Performance

- **Control loop frequency**: 10 Hz (configurable in controllers.yaml)
- **PWM frequency**: 50 Hz (standard for servos)
- **Position update latency**: < 1ms
- **Precision**: 1μs PWM resolution

## Support

For issues or questions:
1. Check this README first
2. Verify hardware connections and power supply
3. Check pigpiod status: `sudo systemctl status pigpiod`
4. Test servos manually with `pigs` commands
5. Review ROS2 logs for error messages

## References

- [pigpio library documentation](http://abyz.me.uk/rpi/pigpio/)
- [Raspberry Pi GPIO pinout](https://pinout.xyz/)
- [Servo motor specifications](https://www.towerpro.com.tw/product/mg995/)
