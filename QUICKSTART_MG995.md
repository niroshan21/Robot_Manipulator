# Quick Start Guide - MG995 Servos with Raspberry Pi GPIO

## Your Setup
- **Servos**: MG995 (optimized PWM: 1000-2000μs)
- **Control**: Raspberry Pi GPIO (no Arduino needed)
- **Launch**: `ros2 launch manipulator_remote real_robot_lightweight.launch.py`
- **Control**: `ros2 action send_goal /task_server ...`

---

## 🚀 First-Time Setup (One Time Only)

### 1. Install GPIO Library
```bash
cd ~/Robot_Manipulator
sudo ./setup_rpi_gpio.sh
```

### 2. Verify pigpiod Service
```bash
sudo systemctl status pigpiod
# Should show "active (running)"

# If not running:
sudo systemctl start pigpiod
sudo systemctl enable pigpiod  # Auto-start on boot
```

### 3. Wire Your MG995 Servos

**GPIO Connections:**
```
Base Servo     → GPIO 17 (Pin 11) [Orange wire]
Shoulder Servo → GPIO 18 (Pin 12) [Orange wire]  
Elbow Servo    → GPIO 27 (Pin 13) [Orange wire]
Gripper Servo  → GPIO 22 (Pin 15) [Orange wire]
```

**Power Connections:**
```
All Servo VCC (Red)    → External 5-6V Power Supply (+)
All Servo GND (Brown)  → Common Ground
Raspberry Pi GND       → Common Ground
PSU Ground (-)         → Common Ground
```

⚠️ **CRITICAL**: Never power servos from Pi's 5V pin!

### 4. Build Your Workspace
```bash
cd ~/Robot_Manipulator
colcon build --packages-select manipulator_msgs manipulator_description manipulator_controller manipulator_remote
source install/setup.bash
```

---

## 🎮 Daily Usage

### Start the Robot
```bash
cd ~/Robot_Manipulator
source install/setup.bash
ros2 launch manipulator_remote real_robot_lightweight.launch.py
```

You should see:
```
[INFO] [ManipulatorInterfaceGPIO]: Initializing GPIO hardware...
[INFO] [ManipulatorInterfaceGPIO]: GPIO pin 17 initialized with neutral position (MG995)
[INFO] [ManipulatorInterfaceGPIO]: GPIO pin 18 initialized with neutral position (MG995)
[INFO] [ManipulatorInterfaceGPIO]: GPIO pin 27 initialized with neutral position (MG995)
[INFO] [ManipulatorInterfaceGPIO]: GPIO pin 22 initialized with neutral position (MG995)
[INFO] [ManipulatorInterfaceGPIO]: GPIO hardware initialized, ready to take commands
```

### Send Task Commands (New Terminal)

Open another terminal:
```bash
cd ~/Robot_Manipulator
source install/setup.bash

# Execute pre-programmed tasks:
ros2 action send_goal /task_server manipulator_msgs/action/ManipulatorTask "{task_number: 0}"
ros2 action send_goal /task_server manipulator_msgs/action/ManipulatorTask "{task_number: 1}"
ros2 action send_goal /task_server manipulator_msgs/action/ManipulatorTask "{task_number: 2}"
```

---

## 🔧 MG995 Servo Specifications

Your code is now optimized for MG995 servos:

| Parameter | Value | Notes |
|-----------|-------|-------|
| PWM Frequency | 50 Hz | Standard (20ms period) |
| Min Pulse | 1000μs | 0° position |
| Center Pulse | 1500μs | 90° position (neutral) |
| Max Pulse | 2000μs | 180° position |
| Operating Voltage | 4.8-6V | Use 5V or 6V supply |
| Stall Current | ~1A | Supply must handle 3-4A total |
| Operating Speed | 0.17 sec/60° @ 4.8V | |
| Torque | 11 kg·cm @ 4.8V | 13 kg·cm @ 6V |

---

## 🐛 Troubleshooting

### No Servo Movement

**Check pigpiod:**
```bash
sudo systemctl status pigpiod
# If not running:
sudo systemctl start pigpiod
```

**Test individual servo:**
```bash
# Move base servo to center (1500μs)
pigs s 17 1500

# Move to 0° (1000μs)
pigs s 17 1000

# Move to 180° (2000μs)
pigs s 17 2000
```

### Servos Jitter or Act Erratically

1. **Check Power Supply:**
   - Voltage: 5-6V DC
   - Current capacity: 3A minimum (4A recommended)
   - Use multimeter to verify voltage under load

2. **Add Capacitor:**
   - 470μF-1000μF capacitor across power supply terminals
   - Reduces voltage spikes when servos move

3. **Shorten Signal Wires:**
   - Keep GPIO signal wires under 20cm if possible
   - Use shielded cable for longer runs

### "Failed to initialize pigpio" Error

```bash
# Ensure pigpiod daemon is running:
sudo systemctl start pigpiod

# Check if it's actually running:
ps aux | grep pigpiod

# Restart if needed:
sudo killall pigpiod
sudo pigpiod
```

### Permission Denied

```bash
# Add user to gpio group:
sudo usermod -a -G gpio $USER

# Then log out and back in, or reboot
```

---

## 📊 Pre-Programmed Tasks

The lightweight mode uses pre-saved trajectories. Check what tasks are available:

```bash
ls ~/Robot_Manipulator/savedTrajectories/
```

Each task number corresponds to a saved trajectory file.

---

## ⚡ Quick Command Reference

| Command | Purpose |
|---------|---------|
| `sudo systemctl status pigpiod` | Check GPIO daemon |
| `sudo systemctl start pigpiod` | Start GPIO daemon |
| `pigs s <gpio> <pulse>` | Test servo manually |
| `ros2 launch ... real_robot_lightweight.launch.py` | Start robot |
| `ros2 action send_goal /task_server ...` | Execute task |
| `ros2 topic echo /joint_states` | Monitor joint positions |

---

## 🔄 Switching GPIO Pins (Optional)

If you need different GPIO pins, edit:
```bash
nano ~/Robot_Manipulator/src/manipulator_description/urdf/manipulator_ros2_control.xacro
```

Find and modify:
```xml
<param name="gpio_base">17</param>      <!-- Your base servo GPIO -->
<param name="gpio_shoulder">18</param>  <!-- Your shoulder servo GPIO -->
<param name="gpio_elbow">27</param>     <!-- Your elbow servo GPIO -->
<param name="gpio_gripper">22</param>   <!-- Your gripper servo GPIO -->
```

Then rebuild:
```bash
colcon build --packages-select manipulator_description
source install/setup.bash
```

---

## ✅ Pre-Flight Checklist

Before each session:
- [ ] External 5-6V power supply connected and ON
- [ ] All servos connected to correct GPIO pins
- [ ] Common ground connected (Pi + Servos + PSU)
- [ ] pigpiod daemon running: `sudo systemctl status pigpiod`
- [ ] Workspace sourced: `source install/setup.bash`

---

## 📞 Need Help?

1. Check this guide first
2. Review [README_RPI_GPIO.md](README_RPI_GPIO.md) for detailed info
3. Test servos manually with `pigs` commands
4. Check ROS logs for specific errors

---

**🎉 Your robot is ready to go!**

Start with: `ros2 launch manipulator_remote real_robot_lightweight.launch.py`
