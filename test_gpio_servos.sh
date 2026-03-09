#!/bin/bash

###############################################################################
# Quick GPIO Pin Test Script
# Use this to quickly test if your servo connections are working
###############################################################################

echo "========================================="
echo "Servo GPIO Test Utility"
echo "========================================="
echo ""

# Check if pigpiod is running
if ! systemctl is-active --quiet pigpiod; then
    echo "❌ Error: pigpiod daemon is not running"
    echo "   Start it with: sudo systemctl start pigpiod"
    exit 1
fi

echo "Testing GPIO servo connections..."
echo ""
echo "This will move each servo to its center position (90 degrees)"
echo "Watch your servos to verify they're connected correctly"
echo ""
read -p "Press Enter to continue (Ctrl+C to abort)..."
echo ""

# GPIO pins (match your URDF configuration)
GPIO_BASE=17
GPIO_SHOULDER=18
GPIO_ELBOW=27
GPIO_GRIPPER=22

# PWM values
CENTER=1500  # 90 degrees
MIN=500      # 0 degrees  
MAX=2500     # 180 degrees

echo "1. Moving BASE servo (GPIO $GPIO_BASE) to center..."
pigs s $GPIO_BASE $CENTER
sleep 1

echo "2. Moving SHOULDER servo (GPIO $GPIO_SHOULDER) to center..."
pigs s $GPIO_SHOULDER $CENTER
sleep 1

echo "3. Moving ELBOW servo (GPIO $GPIO_ELBOW) to center..."
pigs s $GPIO_ELBOW $CENTER
sleep 1

echo "4. Moving GRIPPER servo (GPIO $GPIO_GRIPPER) to center..."
pigs s $GPIO_GRIPPER $CENTER
sleep 1

echo ""
echo "✅ All servos should now be at center position (90°)"
echo ""
read -p "Did all servos move correctly? (y/n) " -n 1 -r
echo ""

if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo ""
    echo "Great! Now testing full range of motion..."
    echo ""
    
    for GPIO in $GPIO_BASE $GPIO_SHOULDER $GPIO_ELBOW $GPIO_GRIPPER; do
        echo "Testing GPIO $GPIO:"
        echo "  -> 0 degrees"
        pigs s $GPIO $MIN
        sleep 1
        echo "  -> 90 degrees"
        pigs s $GPIO $CENTER
        sleep 1
        echo "  -> 180 degrees"
        pigs s $GPIO $MAX
        sleep 1
        echo "  -> back to center"
        pigs s $GPIO $CENTER
        sleep 0.5
    done
    
    echo ""
    echo "✅ Test complete!"
    echo ""
    echo "If servos moved smoothly, your hardware is ready!"
    echo "If any servo didn't move, check:"
    echo "  - Power supply (5-6V, 2A+)"
    echo "  - Signal wire connections"
    echo "  - Common ground connection"
else
    echo ""
    echo "⚠️  Troubleshooting tips:"
    echo ""
    echo "If servos didn't move:"
    echo "  1. Check power supply (servos need 5-6V, 2A+)"
    echo "  2. Verify signal wire connections to GPIO pins"
    echo "  3. Ensure common ground (Pi GND ↔ Servo GND ↔ PSU GND)"
    echo "  4. Try testing individual servos with:"
    echo "     pigs s <gpio_pin> 1500"
    echo ""
    echo "If servos moved to wrong positions:"
    echo "  - Signal wires may be swapped"
    echo "  - Check GPIO pin assignments in URDF"
fi

echo ""
echo "To turn off all servos:"
echo "  pigs s $GPIO_BASE 0"
echo "  pigs s $GPIO_SHOULDER 0"
echo "  pigs s $GPIO_ELBOW 0"
echo "  pigs s $GPIO_GRIPPER 0"
echo ""
