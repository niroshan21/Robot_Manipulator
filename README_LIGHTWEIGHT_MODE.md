# Lightweight Mode for Raspberry Pi

This guide explains how to use pre-generated trajectories on resource-constrained hardware like Raspberry Pi, avoiding the computational overhead of MoveIt.

## Overview

The lightweight mode uses pre-generated trajectory files instead of running MoveIt for motion planning. This significantly reduces CPU and memory usage, making it suitable for Raspberry Pi.

## What's Included

### 1. **task_server_lightweight.py** 

This is the core component that replaces MoveIt-based planning with trajectory playback. When the node starts, it automatically scans the `savedTrajectories/` directory and loads all pre-generated trajectory files into memory as Python dictionaries. Each trajectory file contains a complete motion plan with joint positions, velocities, accelerations, and timing information for every waypoint. The server creates two action clients - one for the arm controller and one for the gripper controller. These clients communicate with the ros2_control framework that manages the actual hardware. When a task request comes in, the server doesn't do any motion planning. Instead, it simply looks up the corresponding trajectory from its loaded dictionary, converts the JSON data into ROS JointTrajectory messages, and sends them to the appropriate controllers for execution.

This approach is fundamentally different from the original task_server.py which uses MoveIt's planning algorithms. Instead of computing collision-free paths in real-time (which requires significant CPU and memory), this lightweight version just plays back pre-validated trajectories. 

### 2. **remote_interface_lightweight.launch.py** 

This launch file is stripped down to the bare essentials needed for trajectory execution. Unlike the original remote_interface.launch.py which initializes the entire MoveIt stack (including move_group, planning scene, collision checking, and the planning pipeline), this lightweight version only starts the task_server_lightweight.py node.

### 3. **real_robot_lightweight.launch.py** 

This is a complete launch file designed specifically for resource-constrained hardware like Raspberry Pi. It orchestrates two main components: the controller system (which interfaces with the Arduino-based hardware) and the lightweight task server (which manages trajectory playback).

When you run this launch file, it first brings up the robot_state_publisher (for kinematic transforms), the ros2_control node (for hardware interface), and spawns the joint_state_broadcaster, arm_controller, and gripper_controller. Once all controllers are active and ready, the task server starts and loads trajectories. The entire system is operational without ever loading MoveIt, making it suitable for deployment on embedded systems with limited RAM and processing power. 

### Pre-Generated Trajectories

Each task has two trajectory files:
- `task_X_arm_trajectory.json` - Arm motion
- `task_X_gripper_trajectory.json` - Gripper motion


## Run on RPI

```bash
# First, connect Arduino and upload firmware

# Source the workspace
cd ~/Robot_Manipulator
. install/setup.bash

# Launch the real robot (uses lightweight mode by default)
ros2 launch manipulator_remote real_robot_lightweight.launch.py
```

### Send Task Commands

From another terminal (can be on RPI or remote machine):

```bash
# Source workspace
. install/setup.bash

# Execute tasks
ros2 action send_goal /task_server manipulator_msgs/action/ManipulatorTask "{task_number: 0}"

```

## Generating Trajectories

### On Development Machine (with MoveIt):

```bash
cd ~/Robot_Manipulator
. install/setup.bash

# Generate trajectories
ros2 launch python_scripts generate_trajectories.launch.py
```

This will create trajectory files in `savedTrajectories/`.

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

## Resource Comparison

| Aspect | Full Mode (MoveIt) | Lightweight Mode |
|--------|-------------------|------------------|
| **Memory** | ~800MB - 1.5GB | ~150-300MB |
| **CPU Usage** | High (spikes to 100%) | Minimal (<5%) |
| **Startup Time** | 10-30 seconds | 2-5 seconds |
| **Requirements** | ROS2 + MoveIt2 + OMPL | ROS2 base + ros2_control |

**Why the difference?** MoveIt maintains complex data structures for real-time planning (robot model, collision meshes, planning scene, OMPL algorithms), while lightweight mode only stores pre-computed trajectories as simple dictionaries and plays them back. This trades flexibility for efficiency - perfect for repetitive tasks with known motions.

## Troubleshooting

### Controller Not Ready
```
Waiting for arm controller...
```
**Solution**: Make sure controller nodes are running. Check that Arduino is connected and firmware is uploaded.

