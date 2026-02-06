#include <Servo.h>

/* ================= CONFIG ================= */

#define SERVO_PIN 8
#define START_POSITION 90

/* ================= GLOBALS ================= */

Servo base;

char value[4] = "000";
uint8_t val_idx = 0;
bool reading_base = false;

/* ================= SMOOTH MOVE ================= */

void reach_goal(Servo& motor, int goal)
{
  goal = constrain(goal, 0, 180);
  int current = motor.read();

  if (goal >= current)
  {
    for (int pos = current; pos <= goal; pos++)
    {
      motor.write(pos);
      delay(5);
    }
  }
  else
  {
    for (int pos = current; pos >= goal; pos--)
    {
      motor.write(pos);
      delay(5);
    }
  }
}

/* ================= SETUP ================= */

void setup()
{
  base.attach(SERVO_PIN);
  base.write(START_POSITION);

  Serial.begin(115200);
  Serial.setTimeout(1);
}

/* ================= LOOP ================= */

void loop()
{
  while (Serial.available())
  {
    char chr = Serial.read();

    // Start reading BASE value
    if (chr == 'e')
    {
      reading_base = true;
      val_idx = 0;
    }
    // Ignore other joints
    else if (chr == 'b' || chr == 's' || chr == 'g')
    {
      reading_base = false;
    }
    // End of value
    else if (chr == ',' && reading_base)
    {
      value[val_idx] = '\0';
      int angle = atoi(value);
      reach_goal(base, angle);

      // Reset buffer
      val_idx = 0;
      reading_base = false;
    }
    // Read numeric characters
    else if (reading_base && isDigit(chr))
    {
      if (val_idx < 3)
      {
        value[val_idx++] = chr;
      }
    }
  }
}
