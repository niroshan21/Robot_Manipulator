#!/usr/bin/env python3

"""
Visualize Servo Trajectories

Generate plots for servo PWM trajectories including:
- PWM pulse widths over time
- Joint angles over time
- Velocities and accelerations
"""

import json
import os
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
import numpy as np


class ServoTrajectoryVisualizer:
    """Create visualizations for servo trajectories"""
    
    def __init__(self, workspace_root=None):
        """Initialize the visualizer
        
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
        self.trajectory_dir = os.path.join(workspace_root, 'servoTrajectories')
        self.output_dir = os.path.join(workspace_root, 'servoTrajectories', 'plots')
        
        # Create output directory
        os.makedirs(self.output_dir, exist_ok=True)
    
    def load_trajectory(self, filename='servo_trajectories.json'):
        """Load servo trajectory data
        
        Args:
            filename: JSON file with trajectory data
            
        Returns:
            Dictionary with trajectory data
        """
        filepath = os.path.join(self.trajectory_dir, filename)
        
        if not os.path.exists(filepath):
            raise FileNotFoundError(f"Trajectory file not found: {filepath}")
        
        with open(filepath, 'r') as f:
            data = json.load(f)
        
        print(f"Loaded trajectory: {data['metadata']['name']}")
        print(f"Number of points: {data['metadata']['num_points']}")
        
        return data
    
    def plot_pwm_trajectories(self, data):
        """Plot PWM pulse widths over time for all joints
        
        Args:
            data: Trajectory data dictionary
        """
        metadata = data['metadata']
        trajectories = data['trajectories']
        joint_names = metadata['joint_names']
        
        # Extract data
        times = [p['time_sec'] for p in trajectories]
        
        # Create figure with subplots
        fig, axes = plt.subplots(4, 1, figsize=(12, 10))
        fig.suptitle('Servo PWM Trajectories', fontsize=16, fontweight='bold')
        
        colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']
        
        for i, (joint_name, ax, color) in enumerate(zip(joint_names, axes, colors)):
            pwm_values = [p['pulse_widths_us'][i] for p in trajectories]
            
            ax.plot(times, pwm_values, color=color, linewidth=2, label=joint_name)
            ax.axhline(y=metadata['pwm_min_us'], color='gray', linestyle='--', alpha=0.5, label='PWM Min')
            ax.axhline(y=metadata['pwm_max_us'], color='gray', linestyle='--', alpha=0.5, label='PWM Max')
            ax.axhline(y=1500, color='gray', linestyle=':', alpha=0.5, label='Center')
            
            ax.set_ylabel('PWM (µs)', fontweight='bold')
            ax.set_ylim([metadata['pwm_min_us'] - 100, metadata['pwm_max_us'] + 100])
            ax.grid(True, alpha=0.3)
            ax.legend(loc='upper right')
            ax.set_title(f'{joint_name} - PWM Pulse Width')
        
        axes[-1].set_xlabel('Time (seconds)', fontweight='bold')
        
        plt.tight_layout()
        
        output_path = os.path.join(self.output_dir, 'pwm_trajectories.png')
        plt.savefig(output_path, dpi=300, bbox_inches='tight')
        print(f"Saved: {output_path}")
        
        return fig
    
    def plot_angle_trajectories(self, data):
        """Plot joint angles over time
        
        Args:
            data: Trajectory data dictionary
        """
        metadata = data['metadata']
        trajectories = data['trajectories']
        joint_names = metadata['joint_names']
        
        # Extract data
        times = [p['time_sec'] for p in trajectories]
        
        # Create figure with subplots
        fig, axes = plt.subplots(4, 1, figsize=(12, 10))
        fig.suptitle('Joint Angle Trajectories', fontsize=16, fontweight='bold')
        
        colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']
        
        for i, (joint_name, ax, color) in enumerate(zip(joint_names, axes, colors)):
            angles_deg = [p['positions_deg'][i] for p in trajectories]
            
            ax.plot(times, angles_deg, color=color, linewidth=2, label=joint_name)
            ax.axhline(y=0, color='gray', linestyle='--', alpha=0.5)
            
            ax.set_ylabel('Angle (°)', fontweight='bold')
            ax.grid(True, alpha=0.3)
            ax.legend(loc='upper right')
            ax.set_title(f'{joint_name} - Joint Angle')
        
        axes[-1].set_xlabel('Time (seconds)', fontweight='bold')
        
        plt.tight_layout()
        
        output_path = os.path.join(self.output_dir, 'angle_trajectories.png')
        plt.savefig(output_path, dpi=300, bbox_inches='tight')
        print(f"Saved: {output_path}")
        
        return fig
    
    def plot_velocities(self, data):
        """Plot joint velocities over time
        
        Args:
            data: Trajectory data dictionary
        """
        metadata = data['metadata']
        trajectories = data['trajectories']
        joint_names = metadata['joint_names']
        
        # Extract data
        times = [p['time_sec'] for p in trajectories]
        
        # Create figure with subplots
        fig, axes = plt.subplots(4, 1, figsize=(12, 10))
        fig.suptitle('Joint Velocity Trajectories', fontsize=16, fontweight='bold')
        
        colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']
        
        for i, (joint_name, ax, color) in enumerate(zip(joint_names, axes, colors)):
            velocities = [p['velocities'][i] for p in trajectories]
            
            ax.plot(times, velocities, color=color, linewidth=2, label=joint_name)
            ax.axhline(y=0, color='gray', linestyle='--', alpha=0.5)
            
            ax.set_ylabel('Velocity (rad/s)', fontweight='bold')
            ax.grid(True, alpha=0.3)
            ax.legend(loc='upper right')
            ax.set_title(f'{joint_name} - Joint Velocity')
        
        axes[-1].set_xlabel('Time (seconds)', fontweight='bold')
        
        plt.tight_layout()
        
        output_path = os.path.join(self.output_dir, 'velocity_trajectories.png')
        plt.savefig(output_path, dpi=300, bbox_inches='tight')
        print(f"Saved: {output_path}")
        
        return fig
    
    def plot_all_joints_combined(self, data):
        """Plot all joints on single graphs for comparison
        
        Args:
            data: Trajectory data dictionary
        """
        metadata = data['metadata']
        trajectories = data['trajectories']
        joint_names = metadata['joint_names']
        
        # Extract data
        times = [p['time_sec'] for p in trajectories]
        
        # Create figure with 3 subplots
        fig, axes = plt.subplots(3, 1, figsize=(14, 10))
        fig.suptitle('All Joints - Combined View', fontsize=16, fontweight='bold')
        
        colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']
        
        # PWM plot
        ax = axes[0]
        for i, (joint_name, color) in enumerate(zip(joint_names, colors)):
            pwm_values = [p['pulse_widths_us'][i] for p in trajectories]
            ax.plot(times, pwm_values, color=color, linewidth=2, label=joint_name)
        
        ax.axhline(y=metadata['pwm_min_us'], color='gray', linestyle='--', alpha=0.3)
        ax.axhline(y=metadata['pwm_max_us'], color='gray', linestyle='--', alpha=0.3)
        ax.axhline(y=1500, color='gray', linestyle=':', alpha=0.3)
        ax.set_ylabel('PWM (µs)', fontweight='bold')
        ax.set_title('PWM Pulse Widths')
        ax.grid(True, alpha=0.3)
        ax.legend(loc='best', ncol=4)
        
        # Angle plot
        ax = axes[1]
        for i, (joint_name, color) in enumerate(zip(joint_names, colors)):
            angles_deg = [p['positions_deg'][i] for p in trajectories]
            ax.plot(times, angles_deg, color=color, linewidth=2, label=joint_name)
        
        ax.axhline(y=0, color='gray', linestyle='--', alpha=0.3)
        ax.set_ylabel('Angle (°)', fontweight='bold')
        ax.set_title('Joint Angles')
        ax.grid(True, alpha=0.3)
        ax.legend(loc='best', ncol=4)
        
        # Velocity plot
        ax = axes[2]
        for i, (joint_name, color) in enumerate(zip(joint_names, colors)):
            velocities = [p['velocities'][i] for p in trajectories]
            ax.plot(times, velocities, color=color, linewidth=2, label=joint_name)
        
        ax.axhline(y=0, color='gray', linestyle='--', alpha=0.3)
        ax.set_ylabel('Velocity (rad/s)', fontweight='bold')
        ax.set_xlabel('Time (seconds)', fontweight='bold')
        ax.set_title('Joint Velocities')
        ax.grid(True, alpha=0.3)
        ax.legend(loc='best', ncol=4)
        
        plt.tight_layout()
        
        output_path = os.path.join(self.output_dir, 'combined_trajectories.png')
        plt.savefig(output_path, dpi=300, bbox_inches='tight')
        print(f"Saved: {output_path}")
        
        return fig
    
    def plot_pwm_statistics(self, data):
        """Plot PWM statistics and distribution
        
        Args:
            data: Trajectory data dictionary
        """
        metadata = data['metadata']
        trajectories = data['trajectories']
        joint_names = metadata['joint_names']
        
        fig = plt.figure(figsize=(14, 8))
        gs = gridspec.GridSpec(2, 4, figure=fig)
        
        fig.suptitle('PWM Statistics and Distribution', fontsize=16, fontweight='bold')
        
        colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']
        
        for i, (joint_name, color) in enumerate(zip(joint_names, colors)):
            pwm_values = [p['pulse_widths_us'][i] for p in trajectories]
            
            # Histogram
            ax_hist = fig.add_subplot(gs[0, i])
            ax_hist.hist(pwm_values, bins=30, color=color, alpha=0.7, edgecolor='black')
            ax_hist.axvline(x=np.mean(pwm_values), color='red', linestyle='--', 
                           linewidth=2, label=f'Mean: {np.mean(pwm_values):.0f}')
            ax_hist.set_xlabel('PWM (µs)')
            ax_hist.set_ylabel('Frequency')
            ax_hist.set_title(f'{joint_name}')
            ax_hist.legend()
            ax_hist.grid(True, alpha=0.3)
            
            # Box plot
            ax_box = fig.add_subplot(gs[1, i])
            bp = ax_box.boxplot([pwm_values], vert=True, patch_artist=True,
                                labels=[joint_name])
            bp['boxes'][0].set_facecolor(color)
            bp['boxes'][0].set_alpha(0.7)
            ax_box.set_ylabel('PWM (µs)')
            ax_box.grid(True, alpha=0.3, axis='y')
            
            # Add statistics text
            stats_text = f"Min: {min(pwm_values)}\n"
            stats_text += f"Max: {max(pwm_values)}\n"
            stats_text += f"Avg: {np.mean(pwm_values):.0f}\n"
            stats_text += f"Std: {np.std(pwm_values):.0f}"
            
            ax_box.text(0.98, 0.98, stats_text, transform=ax_box.transAxes,
                       verticalalignment='top', horizontalalignment='right',
                       bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5),
                       fontsize=8)
        
        plt.tight_layout()
        
        output_path = os.path.join(self.output_dir, 'pwm_statistics.png')
        plt.savefig(output_path, dpi=300, bbox_inches='tight')
        print(f"Saved: {output_path}")
        
        return fig
    
    def generate_all_plots(self):
        """Generate all visualization plots"""
        print("Servo Trajectory Visualizer")
        print("="*60)
        
        # Load data
        data = self.load_trajectory()
        
        print("\nGenerating plots...")
        print("-"*60)
        
        # Generate all plots
        self.plot_pwm_trajectories(data)
        self.plot_angle_trajectories(data)
        self.plot_velocities(data)
        self.plot_all_joints_combined(data)
        self.plot_pwm_statistics(data)
        
        print("-"*60)
        print(f"\n✓ All plots generated successfully!")
        print(f"  Output directory: {self.output_dir}")
        print("="*60)


def main():
    """Main function"""
    visualizer = ServoTrajectoryVisualizer()
    visualizer.generate_all_plots()


if __name__ == '__main__':
    main()
