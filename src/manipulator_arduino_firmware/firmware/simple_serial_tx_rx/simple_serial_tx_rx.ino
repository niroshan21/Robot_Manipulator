/*
 * ROS 2 Safe Serial Interface (Echo + Parse)
 * 
 * Message format (newline terminated):
 * b090,s090,e090,g000\n
 * 
 * This sketch is REAL-TIME SAFE:
 *  - No String usage
 *  - No blocking calls
 *  - Fixed buffers only
 *  - Safe parsing with sscanf
 */

#define RX_BUFFER_SIZE 32

char rx_buffer[RX_BUFFER_SIZE];
uint8_t rx_index = 0;

int base_val = 90;
int shoulder_val = 90;
int elbow_val = 90;
int gripper_val = 0;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  readSerial();
}

/* ================= SERIAL RECEIVE ================= */

void readSerial()
{
  while (Serial.available())
  {
    char c = Serial.read();

    // End of message
    if (c == '\n')
    {
      rx_buffer[rx_index] = '\0';
      processMessage(rx_buffer);
      rx_index = 0;
    }
    // Store character safely
    else if (rx_index < RX_BUFFER_SIZE - 1)
    {
      rx_buffer[rx_index++] = c;
    }
    // Overflow protection
    else
    {
      rx_index = 0;
    }
  }
}

/* ================= MESSAGE PROCESS ================= */

void processMessage(char *msg)
{
  int b, s, e, g;

  // Strict format parsing
  if (sscanf(msg, "b%d,s%d,e%d,g%d", &b, &s, &e, &g) == 4)
  {
    // Clamp values (VERY IMPORTANT)
    base_val     = constrain(b, 0, 180);
    shoulder_val = constrain(s, 0, 180);
    elbow_val    = constrain(e, 0, 180);
    gripper_val  = constrain(g, 0, 180);

    sendAck();
  }
}

/* ================= SERIAL RESPONSE ================= */

void sendAck()
{
  Serial.print("ACK:");
  Serial.print(base_val);     Serial.print(",");
  Serial.print(shoulder_val); Serial.print(",");
  Serial.print(elbow_val);    Serial.print(",");
  Serial.println(gripper_val);
}
