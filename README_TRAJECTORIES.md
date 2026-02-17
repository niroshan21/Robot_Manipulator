# Trajectory Generation and Saving System

This package provides a complete system for generating and saving robot trajectories using MoveIt.

## Features

- **Single Launch File**: Everything starts with one command
- **Automatic Trajectory Generation**: Generates multiple random valid trajectories
- **JSON Storage**: Saves trajectories in human-readable JSON format
- **Configurable**: Easy to customize number of trajectories, planner, and save location
- **Visualization**: Optional RViz2 integration to visualize planning

## Quick Start

### Basic Usage

```bash
# Source your workspace
source install/setup.bash

# Generate 5 trajectories (default)
ros2 launch python_scripts generate_trajectories.launch.py
```

### Advanced Usage

```bash
# Generate 10 trajectories using RRT planner
ros2 launch python_scripts generate_trajectories.launch.py num_trajectories:=10 planner_id:=RRT

# Generate trajectories without RViz
ros2 launch python_scripts generate_trajectories.launch.py use_rviz:=false

# Use different planner and custom save location
ros2 launch python_scripts generate_trajectories.launch.py \
    planner_id:=PRM \
    num_trajectories:=15 \
    save_directory:=my_custom_trajectories

# For real robot (not simulation)
ros2 launch python_scripts generate_trajectories.launch.py is_sim:=false
```

## Launch Arguments

| Argument | Default | Description |
|----------|---------|-------------|
| `is_sim` | `true` | Use simulation time |
| `num_trajectories` | `5` | Number of trajectories to generate |
| `planner_id` | `RRTConnect` | MoveIt planner (RRTConnect, RRT, PRM, etc.) |
| `save_directory` | `savedTrajectories` | Directory to save trajectories |
| `use_rviz` | `true` | Whether to launch RViz2 for visualization |

## Available Planners

- `RRTConnect` - Fast bidirectional RRT (default)
- `RRT` - Basic RRT
- `RRTstar` - Optimal RRT
- `PRM` - Probabilistic Roadmap
- `BKPIECE` - Bi-directional KPIECE
- `LBKPIECE` - Lazy Bi-directional KPIECE

## Trajectory File Format

Trajectories are saved as JSON files with the following structure:

```json
{
  "name": "trajectory_20260217_120000_1",
  "timestamp": "2026-02-17T12:00:00.000000",
  "planning_group": "arm",
  "planner_id": "RRTConnect",
  "joint_names": ["joint_1", "joint_2", "joint_3"],
  "points": [
    {
      "positions": [0.0, 0.0, 0.0],
      "velocities": [0.0, 0.0, 0.0],
      "accelerations": [0.0, 0.0, 0.0],
      "time_from_start": {
        "sec": 0,
        "nanosec": 0
      }
    }
  ]
}
```

## Output Location

Trajectories are saved to: `<workspace_root>/savedTrajectories/`

Each trajectory file is named: `trajectory_<timestamp>_<number>.json`

## What Happens When You Launch

1. **MoveGroup starts** - The core MoveIt planning node initializes
2. **RViz2 launches** - Visualization tool opens (if enabled)
3. **Trajectory Generator starts** - After 5 seconds delay:
   - Generates random valid joint configurations
   - Plans trajectories to each configuration
   - Saves successful trajectories to JSON files
   - Prints summary of success/failure

## Troubleshooting

### No trajectories generated
- Check that MoveGroup is running: `ros2 node list | grep move_group`
- Verify your robot model is loaded: `ros2 topic echo /robot_description`

### Planning failures
- Try different planners: `planner_id:=RRT` or `planner_id:=PRM`
- Check joint limits in your SRDF file
- Reduce number of trajectories for testing

### Save location issues
- Ensure the workspace directory exists
- Check file permissions
- Verify save_directory parameter is correct

## Example Session

```bash
# Terminal 1: Start the system
source install/setup.bash
ros2 launch python_scripts generate_trajectories.launch.py num_trajectories:=3

# Expected output:
# [trajectory_generator]: Initializing MoveItPy...
# [trajectory_generator]: Creating planning component for group: arm
# [trajectory_generator]: ============================================================
# [trajectory_generator]: Starting Trajectory Generation
# [trajectory_generator]: ============================================================
# [trajectory_generator]: Generated goal 1: {'joint_1': 0.5, 'joint_2': -0.3, ...}
# [trajectory_generator]: Planning with RRTConnect...
# [trajectory_generator]: ✓ Saved trajectory to: .../savedTrajectories/trajectory_20260217_120000_1.json
# ...
# [trajectory_generator]: ============================================================
# [trajectory_generator]: Trajectory Generation Complete
# [trajectory_generator]: ============================================================
# [trajectory_generator]: Successful: 3/3
# [trajectory_generator]: Failed: 0/3
```

## Integration with Your Code

To load and use saved trajectories:

```python
import json

# Load trajectory
with open('savedTrajectories/trajectory_20260217_120000_1.json', 'r') as f:
    trajectory_data = json.load(f)

# Access trajectory information
joint_names = trajectory_data['joint_names']
points = trajectory_data['points']

# Use with your robot controller
for point in points:
    positions = point['positions']
    # Send to robot...
```

## Files Created

- `generate_trajectories.py` - Main trajectory generation node
- `generate_trajectories.launch.py` - Launch file for the system
- `README_TRAJECTORIES.md` - This documentation

## Next Steps

- Modify the goal generation to use specific poses instead of random
- Add custom trajectory validation
- Integrate with your existing control system
- Create a trajectory replay system
