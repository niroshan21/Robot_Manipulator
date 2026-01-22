#include <Servo.h>

Servo myServo;   // Create servo object

void setup()
{
  myServo.attach(9);  // Signal wire connected to pin 9
}

void loop()
{
  // Move from 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle++)
  {
    myServo.write(angle);
    delay(15);   // Small delay for smooth motion
  }

  delay(1000);  // Wait 1 second

  // Move from 180 back to 0 degrees
  for (int angle = 180; angle >= 0; angle--)
  {
    myServo.write(angle);
    delay(15);
  }

  delay(1000);  // Wait 1 second
}
