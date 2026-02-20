# Simple Trajectory Generator

**Quick and easy trajectory planning - just specify start and end angles!**

## 🎯 What This Does

1. You specify start and end joint angles in the code
2. System plans a smooth trajectory using MoveIt
3. Trajectory is automatically saved to `savedTrajectories/` folder
4. Ready to use with your robot!

---

## 🚀 Quick Start

### Step 1: Edit Your Angles

Open the generator file:
```bash
nano ~/Robot_Manipulator/src/python_scripts/python_scripts/simple_trajectory_generator.py
```

Find the `main()` function (around line 151) and edit these values:

```python
# ====================================================================
# YOUR TRAJECTORY CONFIGURATION - EDIT HERE!
# ====================================================================

START_ANGLES = [0.0, 0.0, 0.0, 0.0]        # Starting position
END_ANGLES = [-1.14, 1.0, 0.7, 0.5]        # Ending position
TRAJECTORY_NAME = "my_custom_trajectory"    # Name for the saved file

# ====================================================================
```

**Joint Angles Format:** `[joint_1, joint_2, joint_3, joint_4]`

- **joint_1** (base): -1.57 to 1.57 radians (-π/2 to π/2)
- **joint_2** (shoulder): -1.57 to 1.57 radians
- **joint_3** (elbow): -1.57 to 1.57 radians
- **joint_4** (gripper): 0.0 to 0.7 (0 = closed, 0.7 = open)

### Step 2: Build

```bash
cd ~/Robot_Manipulator
colcon build --packages-select python_scripts
source install/setup.bash
```

### Step 3: Generate!

```bash
ros2 launch python_scripts simple_trajectory_generator.launch.py
```

Wait 5-10 seconds for planning to complete. You'll see:
```
✓ TRAJECTORY SAVED!
===========================================================
File:     /home/.../savedTrajectories/my_custom_trajectory.json
Name:     my_custom_trajectory
Points:   247
Duration: 8.25 seconds
Joints:   ['joint_1', 'joint_2', 'joint_3', 'joint_4']
===========================================================

🎉 SUCCESS! 🎉
Your trajectory is ready!
```

Your trajectory file is now in `~/Robot_Manipulator/savedTrajectories/my_custom_trajectory.json`

---

## 📝 Examples

### Example 1: Home to Pick Position
```python
START_ANGLES = [0.0, 0.0, 0.0, 0.0]           # Home position
END_ANGLES = [-1.14, 1.0, 0.7, 0.0]           # Reach forward, gripper closed
TRAJECTORY_NAME = "reach_forward"
```

### Example 2: Pick to Place
```python
START_ANGLES = [-1.14, 1.0, 0.7, 0.0]         # At object, gripper closed
END_ANGLES = [1.57, 0.5, -0.5, 0.7]           # Place position, gripper open
TRAJECTORY_NAME = "pick_to_place"
```

### Example 3: Return Home
```python
START_ANGLES = [1.57, 0.5, -0.5, 0.7]         # After placing
END_ANGLES = [0.0, 0.0, 0.0, 0.0]             # Return home
TRAJECTORY_NAME = "return_home"
```

---

## 🔧 Optional: Generate with Custom Name (No Code Editing)

Instead of editing the Python file, you can pass the name as a parameter:

```bash
ros2 launch python_scripts simple_trajectory_generator.launch.py \
  trajectory_name:="my_special_trajectory"
```

**Note:** You'll still need to edit START_ANGLES and END_ANGLES in the code.

---

## 📊 With RViz Visualization

To see the planning in RViz:

```bash
ros2 launch python_scripts simple_trajectory_generator.launch.py use_rviz:=true
```

---

## 🔍 Checking Your Trajectory

After generation, you can view the JSON file:

```bash
cat ~/Robot_Manipulator/savedTrajectories/my_custom_trajectory.json
```

Or use a JSON viewer:
```bash
python3 -m json.tool ~/Robot_Manipulator/savedTrajectories/my_custom_trajectory.json | less
```

---

## 💡 Tips

1. **Angles in Radians:** Remember all angles are in radians, not degrees!
   - 90° = π/2 ≈ 1.57 radians
   - 180° = π ≈ 3.14 radians

2. **Joint Limits:** Stay within safe ranges or planning will fail:
   ```python
   # Safe ranges for MG995 servos:
   joint_1: -1.57 to 1.57  # ±90°
   joint_2: -1.57 to 1.57  # ±90°
   joint_3: -1.57 to 1.57  # ±90°
   joint_4: 0.0 to 0.7     # gripper
   ```

3. **Velocity/Acceleration:** Trajectory is automatically smoothed based on your settings in:
   - `src/manipulator_moveit/config/joint_limits.yaml`
   
   Current settings (5% speed):
   ```yaml
   default_velocity_scaling_factor: 0.05
   default_acceleration_scaling_factor: 0.05
   ```

4. **Planning Failures:** If planning fails:
   - Check joint limits
   - Ensure angles are reachable
   - Try intermediate waypoints (edit the code to add more steps)

---

## 🎮 Using Your Trajectory

Once saved, you can use it in your robot code. Example for loading:

```python
import json

# Load your trajectory
with open('savedTrajectories/my_custom_trajectory.json', 'r') as f:
    trajectory = json.load(f)

# Access trajectory points
for point in trajectory['points']:
    positions = point['positions']  # [j1, j2, j3, j4]
    time = point['time_from_start_sec']
    # Send to robot...
```

---

## 🛠️ Troubleshooting

### "Planning failed"
- Check that angles are within joint limits
- Ensure start and end positions are reachable
- Try simpler trajectories first

### "File not found" error
- Make sure you ran `colcon build` after editing
- Check that `savedTrajectories/` folder exists

### "No module named 'moveit'"
- Ensure you sourced the workspace: `source install/setup.bash`
- Check MoveIt is installed properly

---

## 📚 Advanced: Multi-Waypoint Trajectories

If you need to go through multiple waypoints (not just start → end), edit the code to use a list:

```python
def main():
    # ... existing code ...
    
    # Multiple waypoints
    waypoints = [
        [0.0, 0.0, 0.0, 0.0],          # Start
        [-1.14, 1.0, 0.7, 0.0],        # Waypoint 1
        [1.57, 0.5, -0.5, 0.5],        # Waypoint 2
        [0.0, 0.0, 0.0, 0.0]           # End
    ]
    
    # Then plan each segment and concatenate
    # (Use the full generate_trajectories.py for this)
```

For complex multi-waypoint trajectories, use:
```bash
ros2 launch python_scripts generate_trajectories.launch.py
```

---

## ✅ Summary

| What                  | How                                                    |
|-----------------------|--------------------------------------------------------|
| **Edit angles**       | Edit `simple_trajectory_generator.py` (line ~151)     |
| **Build**             | `colcon build --packages-select python_scripts`       |
| **Generate**          | `ros2 launch python_scripts simple_trajectory_generator.launch.py` |
| **Output location**   | `~/Robot_Manipulator/savedTrajectories/`              |
| **File format**       | JSON with positions, velocities, accelerations        |

---

## 📞 Need Help?

- Check that MoveIt is properly configured
- Verify joint limits in `src/manipulator_moveit/config/joint_limits.yaml`
- Test with simple trajectories first (small angle changes)

Happy trajectory planning! 🚀
