#include <Servo.h>

Servo mg995;   // Create a servo object

void setup() {
  mg995.attach(8);    // Signal wire connected to PWM pin 9
  mg995.write(90);    // Move servo to 90 degrees (center position)
}

void loop() {
  // Nothing needed here
}
