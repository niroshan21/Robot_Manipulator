# Manipulator Controller Package

This package contains the ros2_control configuration and hardware interface for the manipulator robot.

## Frequently Asked Questions

### 1. How is URDF used in lightweight mode?

Yes, URDF files are still used in the lightweight launch file, but for different purposes than with MoveIt.

**What uses URDF in lightweight mode:**

- **robot_state_publisher** - Reads URDF to publish TF transforms (joint relationships and link positions)
- **ros2_control** - Reads URDF to configure hardware interfaces and know which joints exist
- **Controllers** - Need to know joint names and limits from URDF

**What URDF is NOT used for (compared to MoveIt mode):**

- ❌ Collision checking
- ❌ Motion planning algorithms
- ❌ Planning scene representation
- ❌ Inverse kinematics calculations

**Key Difference:** In lightweight mode, the URDF is only used for basic robot description and control, not for planning. This is much less resource-intensive because you're not maintaining collision meshes or running planning algorithms - you're just using the URDF to understand the robot's joint structure for sending pre-computed commands.

---

### 2. What does `open_loop_control: true` mean?

The `open_loop_control` parameter in [manipulator_controllers.yaml](config/manipulator_controllers.yaml) determines whether the controller uses feedback correction.

**Open-loop (`true`):**
- Controller sends position commands: "Go to position X"
- Assumes hardware will reach that position
- Does NOT read sensor feedback to verify or correct
- Does NOT use PID control to minimize errors
- Simpler, less CPU usage

**Closed-loop (`false`):**
- Controller sends position commands: "Go to position X"
- Continuously reads actual position from sensors
- Calculates error = (desired - actual)
- Uses PID gains to actively correct the error
- More accurate but requires more computation

**In your system:**

With `open_loop_control: true`, the ros2_control trajectory controller simply forwards position targets to your Arduino. The Arduino is responsible for actually moving the servos to those positions. The ROS controller doesn't verify if the servo reached the target or try to correct it - it trusts your Arduino firmware to handle that.

**Why this is appropriate:**
1. Your Arduino firmware has its own servo control
2. Reduces computational load on the Raspberry Pi
3. Simpler for systems with "smart" hardware that handles low-level control

**Note:** You may see a deprecation warning suggesting to use PID gains instead, but for your lightweight RPI setup, keeping it as `true` is fine.

---

### 3. Does Gazebo simulation use open-loop control?

Yes, with the current configuration (`open_loop_control: true`), it's used for **both** Gazebo simulation and real hardware because you're using the same controller configuration file.

**How it works in each case:**

#### In Gazebo Simulation:
- ROS trajectory controller sends position targets
- Gazebo's internal physics engine moves the simulated joints toward those positions
- **Gazebo has its own joint controllers that try to reach the commanded position**
- ROS doesn't use feedback to correct errors (because open_loop is true)

#### On Real Hardware (Arduino):
- ROS trajectory controller sends position targets to Arduino
- Arduino moves servos to those positions
- ROS doesn't use feedback to correct errors

**Could you use closed-loop in Gazebo?**

Yes, you could set `open_loop_control: false` for Gazebo to get more realistic PID-based control, but you'd need:
- Separate configuration files for simulation vs real hardware
- PID gain tuning (`gains:` parameters in YAML)
- More CPU usage

---

## Package Structure

```
manipulator_controller/
├── config/
│   └── manipulator_controllers.yaml    # Controller configuration
├── include/
│   └── manipulator_controller/
│       └── manipulator_interface.hpp   # Hardware interface header
├── src/
│   └── manipulator_interface.cpp       # Hardware interface implementation
├── launch/
│   └── controller.launch.py            # Launch file for controllers
├── CMakeLists.txt
├── package.xml
└── README.md
```

## Controller Configuration

The main configuration file is [config/manipulator_controllers.yaml](config/manipulator_controllers.yaml), which defines:

- **controller_manager**: Update rate and controller types
- **arm_controller**: Joint trajectory controller for joints 1-3
- **gripper_controller**: Joint trajectory controller for joint 4
- **joint_state_broadcaster**: Publishes joint states for visualization


## Hardware Interface

The `ManipulatorInterface` class provides the bridge between ros2_control and your hardware (Arduino). It implements:

- `on_init()`: Initialize hardware connection
- `on_configure()`: Configure hardware parameters
- `on_activate()`: Start hardware interface
- `read()`: Read joint states from hardware
- `write()`: Send commands to hardware
