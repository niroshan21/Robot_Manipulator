# Lightweight Mode for Raspberry Pi

This guide explains how to use pre-generated trajectories on resource-constrained hardware like Raspberry Pi, avoiding the computational overhead of MoveIt.

## Overview

The lightweight mode uses pre-generated trajectory files instead of running MoveIt for motion planning. This significantly reduces CPU and memory usage, making it suitable for Raspberry Pi.

## What's Included

1. **task_server_lightweight.py** - Lightweight task server that loads and executes pre-generated trajectories
2. **remote_interface_lightweight.launch.py** - Launch file without MoveIt dependencies
3. **real_robot.launch.py** - Updated to use lightweight mode by default

## Pre-Generated Trajectories

The system uses 3 pre-generated trajectories saved in `savedTrajectories/`:

- **Task 0**: Home position [0.0, 0.0, 0.0], gripper open [0.7, -0.7]
- **Task 1**: Pick/place position [-1.14, 1.0, 0.7], gripper closed [0.0, 0.0]
- **Task 2**: Alternative position [1.57, 0.5, -0.5], gripper half-open [0.5, -0.5]

Each task has two trajectory files:
- `task_X_arm_trajectory.json` - Arm motion
- `task_X_gripper_trajectory.json` - Gripper motion

## Setup on Raspberry Pi

### 1. Copy Required Files to RPI

Transfer these directories to your Raspberry Pi:
```bash
# On your development machine, create a transfer package
cd ~/Robot_Manipulator
tar -czf rpi_package.tar.gz \
    savedTrajectories/ \
    src/manipulator_remote/ \
    src/manipulator_controller/ \
    src/manipulator_description/ \
    src/manipulator_msgs/

# Copy to RPI (adjust IP address)
scp rpi_package.tar.gz pi@raspberrypi.local:~/

# On RPI, extract
ssh pi@raspberrypi.local
cd ~
tar -xzf rpi_package.tar.gz
```

### 2. Build on RPI (Without MoveIt)

```bash
cd ~/Robot_Manipulator
colcon build --packages-select \
    manipulator_msgs \
    manipulator_description \
    manipulator_controller \
    manipulator_remote
```

Note: This build does NOT require MoveIt packages, making it much faster and lighter.

### 3. Run on RPI

```bash
# First, connect Arduino and upload firmware

# Source the workspace
cd ~/Robot_Manipulator
. install/setup.bash

# Launch the real robot (uses lightweight mode by default)
ros2 launch manipulator_remote real_robot.launch.py
```

### 4. Send Task Commands

From another terminal (can be on RPI or remote machine):

```bash
# Source workspace
. install/setup.bash

# Execute tasks
ros2 action send_goal /task_server manipulator_msgs/action/ManipulatorTask "{task_number: 0}"
ros2 action send_goal /task_server manipulator_msgs/action/ManipulatorTask "{task_number: 1}"
ros2 action send_goal /task_server manipulator_msgs/action/ManipulatorTask "{task_number: 2}"
```

## Generating New Trajectories

If you need to create new trajectories or update existing ones:

### On Development Machine (with MoveIt):

```bash
cd ~/Robot_Manipulator
. install/setup.bash

# Generate trajectories
ros2 run python_scripts generate_trajectories.py
```

This will create/update trajectory files in `savedTrajectories/`.

Then transfer the updated trajectories to RPI:
```bash
scp savedTrajectories/*.json pi@raspberrypi.local:~/Robot_Manipulator/savedTrajectories/
```

## Architecture Comparison

### Full Mode (Development Machine):
```
Task Request → MoveIt Planning → Trajectory → Controllers → Robot
               (Heavy computation)
```

### Lightweight Mode (RPI):
```
Task Request → Load Pre-gen Trajectory → Controllers → Robot
               (No planning, just playback)
```

## Resource Usage

### Full Mode:
- Memory: ~800MB - 1.5GB
- CPU: High during planning
- Requires: Full ROS2, MoveIt, OMPL

### Lightweight Mode:
- Memory: ~150-300MB
- CPU: Minimal
- Requires: Only ROS2 base + controllers

## Troubleshooting

### Trajectories Not Found
```
ERROR: Failed to load trajectories
```
**Solution**: Ensure `savedTrajectories/` is in the workspace root with all 6 JSON files.

### Controller Not Ready
```
Waiting for arm controller...
```
**Solution**: Make sure controller nodes are running. Check that Arduino is connected and firmware is uploaded.

### Task Rejected
```
Rejecting goal: Task X not available
```
**Solution**: The trajectory file for that task is missing. Check `savedTrajectories/` directory.

## Switching Back to Full Mode

If you need to use MoveIt on more powerful hardware:

```bash
# Edit real_robot.launch.py to uncomment MoveIt
# OR use the original remote_interface.launch.py
ros2 launch manipulator_remote remote_interface.launch.py use_python:=True is_sim:=False
```

## Benefits of Lightweight Mode

✅ Much lower memory usage  
✅ Faster startup time  
✅ Consistent execution (pre-validated trajectories)  
✅ Suitable for Raspberry Pi and similar hardware  
✅ No planning delays during execution  

## Limitations

⚠️ Fixed trajectories only (3 tasks)  
⚠️ Cannot adapt to obstacles or new goals  
⚠️ Requires regeneration on development machine for new tasks  
