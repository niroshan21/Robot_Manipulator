#!/usr/bin/env python3

"""
Generate Servo Reference Position Trajectories

This script reads the full_smooth_trajectory.json and converts joint positions 
(in radians) to PWM pulse widths (in microseconds) for servo control.

PWM Parameters (MG995 servos):
- Frequency: 50 Hz (20ms period)
- Pulse width range: 500-2500 microseconds
- 500us = 0°, 1500us = 90°, 2500us = 180°
"""

import json
import os
import math
import csv
from datetime import datetime


class ServoTrajectoryGenerator:
    """Convert joint trajectories to servo PWM pulse width trajectories"""
    
    # PWM parameters for MG995 servos (microseconds)
    PWM_MIN = 500      # Minimum pulse width (0 degrees)
    PWM_MAX = 2500     # Maximum pulse width (180 degrees)
    PWM_CENTER = 1500  # Center position (90 degrees)
    PWM_FREQ = 50      # Standard servo frequency (50Hz = 20ms period)
    
    # Joint limits from URDF (radians)
    JOINT_LIMITS = {
        'joint_1': {'min': -1.57, 'max': 1.57},  # Base: -90° to 90°
        'joint_2': {'min': -0.79, 'max': 1.57},  # Shoulder: -45° to 90°
        'joint_3': {'min': -0.79, 'max': 0.79},  # Elbow: -45° to 45°
        'joint_4': {'min': 0.0, 'max': 0.79},    # Gripper: 0° to 45°
    }
    
    def __init__(self, workspace_root=None):
        """Initialize the generator
        
        Args:
            workspace_root: Path to workspace root (auto-detected if None)
        """
        if workspace_root is None:
            # Auto-detect workspace root
            script_dir = os.path.dirname(os.path.abspath(__file__))
            workspace_root = script_dir
            for _ in range(6):
                workspace_root = os.path.dirname(workspace_root)
                if os.path.exists(os.path.join(workspace_root, 'src')):
                    break
        
        self.workspace_root = workspace_root
        self.trajectory_dir = os.path.join(workspace_root, 'savedTrajectories')
        self.output_dir = os.path.join(workspace_root, 'servoTrajectories')
        
        # Create output directory
        os.makedirs(self.output_dir, exist_ok=True)
    
    def angle_to_pulse_width(self, angle_rad, min_angle, max_angle):
        """Convert angle in radians to PWM pulse width in microseconds
        
        Args:
            angle_rad: Angle in radians
            min_angle: Minimum angle limit in radians
            max_angle: Maximum angle limit in radians
            
        Returns:
            PWM pulse width in microseconds (500-2500)
        """
        # Normalize angle to 0-1 range based on joint limits
        normalized = (angle_rad - min_angle) / (max_angle - min_angle)
        normalized = max(0.0, min(1.0, normalized))  # Clamp to [0, 1]
        
        # Map to PWM pulse width
        pulse_width = self.PWM_MIN + int(normalized * (self.PWM_MAX - self.PWM_MIN))
        
        # Clamp to PWM limits
        return max(self.PWM_MIN, min(self.PWM_MAX, pulse_width))
    
    def convert_trajectory(self, trajectory_file='full_smooth_trajectory.json'):
        """Convert joint trajectory to servo PWM trajectories
        
        Args:
            trajectory_file: Name of trajectory JSON file
            
        Returns:
            Dictionary with servo trajectories
        """
        # Load trajectory
        trajectory_path = os.path.join(self.trajectory_dir, trajectory_file)
        
        if not os.path.exists(trajectory_path):
            raise FileNotFoundError(f"Trajectory file not found: {trajectory_path}")
        
        print(f"Loading trajectory from: {trajectory_path}")
        
        with open(trajectory_path, 'r') as f:
            trajectory = json.load(f)
        
        # Extract data
        joint_names = trajectory['joint_names']
        points = trajectory['points']
        
        print(f"Trajectory: {trajectory['name']}")
        print(f"Joints: {joint_names}")
        print(f"Number of points: {len(points)}")
        
        # Prepare servo trajectory data
        servo_trajectories = {
            'metadata': {
                'name': trajectory['name'],
                'timestamp': datetime.now().isoformat(),
                'pwm_frequency_hz': self.PWM_FREQ,
                'pwm_min_us': self.PWM_MIN,
                'pwm_max_us': self.PWM_MAX,
                'joint_names': joint_names,
                'num_points': len(points)
            },
            'trajectories': []
        }
        
        # Convert each trajectory point
        for i, point in enumerate(points):
            positions_rad = point['positions']
            time_sec = point['time_from_start_sec']
            
            # Calculate pulse widths for each joint
            pulse_widths = []
            positions_deg = []
            
            for j, (joint_name, angle_rad) in enumerate(zip(joint_names, positions_rad)):
                limits = self.JOINT_LIMITS[joint_name]
                pulse_width = self.angle_to_pulse_width(
                    angle_rad, 
                    limits['min'], 
                    limits['max']
                )
                pulse_widths.append(pulse_width)
                positions_deg.append(math.degrees(angle_rad))
            
            servo_trajectories['trajectories'].append({
                'index': i,
                'time_sec': time_sec,
                'positions_rad': positions_rad,
                'positions_deg': positions_deg,
                'pulse_widths_us': pulse_widths,
                'velocities': point.get('velocities', []),
                'accelerations': point.get('accelerations', [])
            })
        
        return servo_trajectories
    
    def save_json(self, servo_trajectories, output_file='servo_trajectories.json'):
        """Save servo trajectories to JSON file
        
        Args:
            servo_trajectories: Dictionary with servo trajectory data
            output_file: Output filename
        """
        output_path = os.path.join(self.output_dir, output_file)
        
        with open(output_path, 'w') as f:
            json.dump(servo_trajectories, f, indent=2)
        
        print(f"\nSaved JSON to: {output_path}")
        return output_path
    
    def save_csv(self, servo_trajectories, output_file='servo_trajectories.csv'):
        """Save servo trajectories to CSV file for easy analysis
        
        Args:
            servo_trajectories: Dictionary with servo trajectory data
            output_file: Output filename
        """
        output_path = os.path.join(self.output_dir, output_file)
        
        joint_names = servo_trajectories['metadata']['joint_names']
        
        with open(output_path, 'w', newline='') as f:
            writer = csv.writer(f)
            
            # Header
            header = ['index', 'time_sec']
            for joint in joint_names:
                header.extend([
                    f'{joint}_rad',
                    f'{joint}_deg',
                    f'{joint}_pwm_us'
                ])
            writer.writerow(header)
            
            # Data rows
            for point in servo_trajectories['trajectories']:
                row = [point['index'], point['time_sec']]
                
                for i in range(len(joint_names)):
                    row.extend([
                        point['positions_rad'][i],
                        point['positions_deg'][i],
                        point['pulse_widths_us'][i]
                    ])
                
                writer.writerow(row)
        
        print(f"Saved CSV to: {output_path}")
        return output_path
    
    def save_compact_csv(self, servo_trajectories, output_file='servo_pwm_compact.csv'):
        """Save compact CSV with only time and PWM values
        
        Args:
            servo_trajectories: Dictionary with servo trajectory data
            output_file: Output filename
        """
        output_path = os.path.join(self.output_dir, output_file)
        
        joint_names = servo_trajectories['metadata']['joint_names']
        
        with open(output_path, 'w', newline='') as f:
            writer = csv.writer(f)
            
            # Header
            header = ['time_sec'] + [f'{joint}_pwm_us' for joint in joint_names]
            writer.writerow(header)
            
            # Data rows
            for point in servo_trajectories['trajectories']:
                row = [point['time_sec']] + point['pulse_widths_us']
                writer.writerow(row)
        
        print(f"Saved compact CSV to: {output_path}")
        return output_path
    
    def print_summary(self, servo_trajectories):
        """Print summary statistics of the trajectory
        
        Args:
            servo_trajectories: Dictionary with servo trajectory data
        """
        metadata = servo_trajectories['metadata']
        trajectories = servo_trajectories['trajectories']
        joint_names = metadata['joint_names']
        
        print("\n" + "="*60)
        print("SERVO TRAJECTORY SUMMARY")
        print("="*60)
        print(f"Trajectory: {metadata['name']}")
        print(f"Total points: {metadata['num_points']}")
        print(f"Duration: {trajectories[-1]['time_sec']:.2f} seconds")
        print(f"PWM Frequency: {metadata['pwm_frequency_hz']} Hz")
        print(f"PWM Range: {metadata['pwm_min_us']}-{metadata['pwm_max_us']} µs")
        
        print("\n" + "-"*60)
        print("JOINT PWM RANGES:")
        print("-"*60)
        
        for i, joint_name in enumerate(joint_names):
            pwm_values = [point['pulse_widths_us'][i] for point in trajectories]
            min_pwm = min(pwm_values)
            max_pwm = max(pwm_values)
            avg_pwm = sum(pwm_values) / len(pwm_values)
            
            # Get corresponding angles
            angles_deg = [point['positions_deg'][i] for point in trajectories]
            min_angle = min(angles_deg)
            max_angle = max(angles_deg)
            
            print(f"{joint_name}:")
            print(f"  PWM:   {min_pwm:4d} - {max_pwm:4d} µs  (avg: {avg_pwm:.0f} µs)")
            print(f"  Angle: {min_angle:6.1f}° - {max_angle:6.1f}°")
        
        print("="*60 + "\n")


def main():
    """Main function to generate servo trajectories"""
    print("Servo Trajectory Generator")
    print("Converting full_smooth_trajectory to PWM pulse widths\n")
    
    # Create generator
    generator = ServoTrajectoryGenerator()
    
    # Convert trajectory
    try:
        servo_trajectories = generator.convert_trajectory('full_smooth_trajectory.json')
        
        # Save outputs
        generator.save_json(servo_trajectories, 'servo_trajectories.json')
        generator.save_csv(servo_trajectories, 'servo_trajectories.csv')
        generator.save_compact_csv(servo_trajectories, 'servo_pwm_compact.csv')
        
        # Print summary
        generator.print_summary(servo_trajectories)
        
        print("\n✓ Servo trajectory generation completed successfully!")
        print(f"  Output directory: {generator.output_dir}")
        
    except Exception as e:
        print(f"\n✗ Error: {e}")
        raise


if __name__ == '__main__':
    main()
