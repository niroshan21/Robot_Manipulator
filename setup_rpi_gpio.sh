#!/bin/bash

###############################################################################
# Raspberry Pi GPIO Setup Script for Manipulator Control
# This script installs and configures the necessary GPIO libraries
###############################################################################

set -e  # Exit on error

echo "========================================="
echo "RPI GPIO Setup for Manipulator Control"
echo "========================================="
echo ""

# Check if running on Raspberry Pi
if ! grep -q "Raspberry Pi" /proc/cpuinfo 2>/dev/null; then
    echo "⚠️  Warning: This doesn't appear to be a Raspberry Pi"
    echo "   GPIO control may not work properly"
    read -p "Continue anyway? (y/n) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# Check if running as root
if [ "$EUID" -ne 0 ]; then 
    echo "❌ This script must be run as root (use sudo)"
    exit 1
fi

echo "📦 Installing pigpio library..."
echo ""

# Update package list
apt-get update

# Install pigpio
apt-get install -y pigpio python3-pigpio

echo ""
echo "⚙️  Configuring pigpio daemon..."
echo ""

# Enable and start pigpiod service
systemctl enable pigpiod
systemctl start pigpiod

# Check if service is running
if systemctl is-active --quiet pigpiod; then
    echo "✅ pigpiod daemon is running"
else
    echo "⚠️  Warning: pigpiod daemon failed to start"
    echo "   You may need to start it manually: sudo systemctl start pigpiod"
fi

echo ""
echo "🔧 Setting up GPIO permissions..."
echo ""

# Add current user to gpio group (if not root)
if [ -n "$SUDO_USER" ]; then
    usermod -a -G gpio $SUDO_USER
    echo "✅ Added user '$SUDO_USER' to gpio group"
    echo "   (You may need to log out and back in for this to take effect)"
fi

echo ""
echo "📋 GPIO Pin Configuration:"
echo "   The following GPIO pins are configured in the URDF:"
echo "   - GPIO 17: Base servo"
echo "   - GPIO 18: Shoulder servo  "
echo "   - GPIO 27: Elbow servo"
echo "   - GPIO 22: Gripper servo"
echo ""
echo "   You can change these in:"
echo "   src/manipulator_description/urdf/manipulator_ros2_control.xacro"
echo ""

echo "⚡ Testing GPIO access..."
echo ""

# Test if we can access GPIO
if command -v pigs &> /dev/null; then
    # Try a simple command
    if pigs hwver &> /dev/null; then
        echo "✅ GPIO access verified"
    else
        echo "⚠️  Could not verify GPIO access"
        echo "   Make sure pigpiod is running: sudo systemctl status pigpiod"
    fi
else
    echo "⚠️  pigs command not found, skipping GPIO test"
fi

echo ""
echo "========================================="
echo "✅ Setup Complete!"
echo "========================================="
echo ""
echo "Next steps:"
echo "1. Wire your servos to the configured GPIO pins"
echo "2. Ensure your servos are powered appropriately (external power recommended)"
echo "3. Build your ROS2 workspace: colcon build"
echo "4. Source your workspace: source install/setup.bash"
echo "5. Launch your robot!"
echo ""
echo "⚠️  IMPORTANT: Make sure pigpiod is running before launching ROS2:"
echo "   sudo systemctl status pigpiod"
echo ""
echo "To manually start pigpiod if needed:"
echo "   sudo systemctl start pigpiod"
echo ""
