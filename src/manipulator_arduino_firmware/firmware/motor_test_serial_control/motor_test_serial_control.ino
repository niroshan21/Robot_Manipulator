#include <Servo.h>

Servo myServo;

void setup()
{
  Serial.begin(9600);
  myServo.attach(9);
  Serial.println("Enter angle (0 - 180):");
}

void loop()
{
  if (Serial.available())
  {
    int angle = Serial.parseInt();
    angle = constrain(angle, 0, 180);
    myServo.write(angle);

    Serial.print("Moved to angle: ");
    Serial.println(angle);
  }
}
