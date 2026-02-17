# Lightweight Mode for Raspberry Pi

This guide explains how to use pre-generated trajectories on resource-constrained hardware like Raspberry Pi, avoiding the computational overhead of MoveIt.

## Overview

The lightweight mode uses pre-generated trajectory files instead of running MoveIt for motion planning. This significantly reduces CPU and memory usage, making it suitable for Raspberry Pi.

## What's Included

### 1. **task_server_lightweight.py** - Lightweight Task Server

This is the core component that replaces MoveIt-based planning with trajectory playback. When the node starts, it automatically scans the `savedTrajectories/` directory and loads all pre-generated trajectory files into memory as Python dictionaries. Each trajectory file contains a complete motion plan with joint positions, velocities, accelerations, and timing information for every waypoint.

The server creates two action clients - one for the arm controller and one for the gripper controller. These clients communicate with the ros2_control framework that manages the actual hardware. When a task request comes in, the server doesn't do any motion planning. Instead, it simply looks up the corresponding trajectory from its loaded dictionary, converts the JSON data into ROS JointTrajectory messages, and sends them to the appropriate controllers for execution.

This approach is fundamentally different from the original task_server.py which uses MoveIt's planning algorithms. Instead of computing collision-free paths in real-time (which requires significant CPU and memory), this lightweight version just plays back pre-validated trajectories. Think of it like the difference between live rendering a 3D animation versus playing back a pre-rendered video file.

### 2. **remote_interface_lightweight.launch.py** - Minimal Launch Configuration

This launch file is stripped down to the bare essentials needed for trajectory execution. Unlike the original remote_interface.launch.py which initializes the entire MoveIt stack (including move_group, planning scene, collision checking, and the planning pipeline), this lightweight version only starts the task_server_lightweight.py node.

The launch file accepts two parameters: `is_sim` (to distinguish between simulation and real hardware) and `trajectory_dir` (the location of saved trajectory files). By eliminating MoveIt dependencies, this launch file can start in seconds rather than the 10-30 seconds typically required for MoveIt initialization, and uses a fraction of the memory.

### 3. **real_robot_lightweight.launch.py** - Complete RPI Launch File

This is a complete launch file designed specifically for resource-constrained hardware like Raspberry Pi. It orchestrates two main components: the controller system (which interfaces with the Arduino-based hardware) and the lightweight task server (which manages trajectory playback).

When you run this launch file, it first brings up the robot_state_publisher (for kinematic transforms), the ros2_control node (for hardware interface), and spawns the joint_state_broadcaster, arm_controller, and gripper_controller. Once all controllers are active and ready, the task server starts and loads trajectories. The entire system is operational without ever loading MoveIt, making it suitable for deployment on embedded systems with limited RAM and processing power. 

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

## Architecture Comparison & Code Flow

### Full Mode (Development Machine):
```
Task Request → MoveIt Planning → Trajectory → Controllers → Robot
               (Heavy computation)
```

In full mode, when you send a task request, the system performs these computationally intensive steps:
1. **Scene Update**: MoveIt updates its internal collision world model
2. **Kinematics Solving**: Computes inverse kinematics for the goal pose
3. **Path Planning**: Uses OMPL algorithms (RRT, PRM, etc.) to find collision-free path
4. **Trajectory Smoothing**: Applies time parameterization and smoothing
5. **Execution**: Sends final trajectory to controllers

This entire process can take 1-5 seconds and requires substantial CPU and memory because MoveIt maintains complex data structures for collision checking, planning scene representation, and kinematic chains.

### Lightweight Mode (RPI):
```
Task Request → Load Pre-gen Trajectory → Controllers → Robot
               (No planning, just playback)
```

In lightweight mode, the process is dramatically simplified:
1. **Dictionary Lookup**: `trajectory = self.trajectories[task_number]` (instant)
2. **JSON Parsing**: Convert stored JSON data to ROS messages (milliseconds)
3. **Controller Command**: Send trajectory directly to hardware controllers
4. **Execution**: Controllers interpolate and execute motion

The key insight here is that all the heavy computation was done once during trajectory generation on a powerful development machine. The Raspberry Pi only needs to deserialize pre-computed data and forward it to controllers - operations that are trivial even for embedded hardware.

## Detailed Code Workflow

### Trajectory Loading Process (Startup)

When `task_server_lightweight.py` initializes, the `load_all_trajectories()` method executes:

```python
# Pseudocode explanation:
for task_num in [0, 1, 2]:
    # Construct file paths
    arm_file = "savedTrajectories/task_0_arm_trajectory.json"
    gripper_file = "savedTrajectories/task_0_gripper_trajectory.json"
    
    # Load JSON files (contains waypoints with positions, velocities, timing)
    with open(arm_file) as f:
        arm_data = json.load(f)  # Dictionary with 'points', 'joint_names', etc.
    
    # Store in memory for instant access
    self.trajectories[0] = {'arm': arm_data, 'gripper': gripper_data}
```

Each JSON file contains a structure like:
```json
{
  "name": "task_0_arm_trajectory",
  "joint_names": ["joint_1", "joint_2", "joint_3"],
  "points": [
    {
      "positions": [0.0, 0.0, 0.0],
      "velocities": [0.0, 0.0, 0.0],
      "time_from_start_sec": 0.0
    },
    // ... more waypoints
  ]
}
```

This data represents a complete motion plan that was generated by MoveIt on your development machine. It includes every intermediate position the joints must pass through, the velocities at each point, and the precise timing.

### Task Execution Process (Runtime)

When you send an action goal like `{task_number: 1}`, here's what happens inside `execute_callback()`:

**Step 1: Trajectory Retrieval**
```python
task_data = self.trajectories[task_num]  # Get pre-loaded data
arm_trajectory = self.json_to_joint_trajectory(task_data['arm'])
```

**Step 2: JSON to ROS Message Conversion**

The `json_to_joint_trajectory()` method transforms JSON data into a ROS `JointTrajectory` message that controllers understand:

```python
# For each waypoint in the JSON:
for point_data in trajectory_data['points']:
    # Create a trajectory point
    point.positions = [0.0, 0.0, 0.0]  # From JSON
    point.velocities = [0.0, 0.0, 0.0]  # From JSON
    point.time_from_start = Duration(sec=2, nanosec=500000000)  # From JSON
    
    trajectory_msg.points.append(point)
```

**Step 3: Sending to Controllers**

The trajectory is wrapped in a `FollowJointTrajectory.Goal` and sent asynchronously:

```python
arm_goal = FollowJointTrajectory.Goal()
arm_goal.trajectory = arm_trajectory
arm_goal.trajectory.header.stamp.sec = 0  # Start immediately

# Send to controller (non-blocking)
arm_future = self.arm_action_client.send_goal_async(arm_goal)
arm_goal_handle = await arm_future  # Wait for acceptance

# Wait for execution to complete
result = await arm_goal_handle.get_result_async()
```

The controller (joint_trajectory_controller) receives this trajectory and performs real-time interpolation between waypoints, generating smooth joint commands at the configured update rate (typically 100-1000 Hz). The controller handles all the low-level details like position interpolation, velocity profiling, and sending commands to the hardware interface.

**Step 4: Sequential Execution**

The code executes arm and gripper movements sequentially (first arm, then gripper) to ensure coordinated motion. In a production system, you might execute them in parallel or with specific timing coordination, but sequential execution is simpler and more predictable for basic pick-and-place operations.

## Resource Usage & Performance

### Full Mode (with MoveIt):
- **Memory**: ~800MB - 1.5GB
  - MoveIt libraries and dependencies: ~400MB
  - Planning scene representation: ~100-200MB
  - OMPL planner states and graphs: ~200-400MB
  - Collision geometry data structures: ~100MB
  
- **CPU Usage**: High during planning (can spike to 100% for 1-5 seconds)
  - Inverse kinematics calculations
  - Collision checking for thousands of states
  - Path optimization algorithms
  - Trajectory smoothing and time parameterization
  
- **Startup Time**: 10-30 seconds to initialize all MoveIt components
  
- **Requirements**: Full ROS2 distribution + MoveIt2 + OMPL planning library

### Lightweight Mode (Trajectory Playback):
- **Memory**: ~150-300MB total
  - ROS2 base libraries: ~100MB
  - Controller manager and plugins: ~30-50MB
  - Trajectory data in RAM: ~1-5MB (6 JSON files)
  - Python interpreter and node: ~20-50MB
  
- **CPU Usage**: Minimal (typically <5% on Raspberry Pi 4)
  - JSON parsing: one-time at startup, negligible
  - Message conversion: <1ms per trajectory
  - No planning algorithms running
  
- **Startup Time**: 2-5 seconds (mostly controller initialization)
  
- **Requirements**: Only ROS2 base packages + ros2_control + trajectory controllers

### Why Such a Huge Difference?

The resource difference comes from what each system needs to maintain in memory and compute in real-time:

**MoveIt maintains:**
- Complete URDF robot model with kinematic chains
- Collision meshes for all robot links
- Octree or voxel grid representation of the environment
- Planning scene with all obstacles
- Multiple planner instances with configuration
- State space samplers and distance metrics

**Lightweight mode maintains:**
- Simple dictionaries with pre-computed waypoints
- Action client connections to controllers
- Current execution state (which trajectory is running)

The lightweight system essentially trades flexibility (ability to plan new motions) for efficiency (minimal resource usage). This is perfect for repetitive tasks where you know the motions in advance.

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
