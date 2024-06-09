#include <Servo.h>

#define WAIST_PIN 11
#define SHOULDER_PIN 9
#define ELBOW_PIN 10
#define WRIST_PIN 6
#define WRIST_TWIST_PIN 3
#define TOOL_PIN 5

#define WAIST_HOME 0
#define SHOULDER_HOME 0
#define ELBOW_HOME 0
#define WRIST_HOME 0
#define WRIST_TWIST_HOME 0
#define TOOL_TIP_HOME 35

Servo waist;
Servo shoulder;
Servo elbow;
Servo wrist;
Servo wrist_twist;
Servo tool_tip;

char received_chars[32];  // Buffer for the received serial data
int char_pos = 0;  // Position in the buffer

void setup() {
  waist.attach(WAIST_PIN);
  shoulder.attach(SHOULDER_PIN);
  elbow.attach(ELBOW_PIN);
  wrist.attach(WRIST_PIN);
  wrist_twist.attach(WRIST_TWIST_PIN);
  tool_tip.attach(TOOL_PIN);

  waist.write(WAIST_HOME);
  shoulder.write(SHOULDER_HOME);
  elbow.write(ELBOW_HOME);
  wrist.write(WRIST_HOME);
  wrist_twist.write(WRIST_TWIST_HOME);
  tool_tip.write(TOOL_TIP_HOME);

  Serial.begin(115200);
  Serial.setTimeout(1);
}

void loop() {
  while (Serial.available()) {
    char chr = Serial.read();
    if (chr == ',') {
      received_chars[char_pos] = '\0';  // Null-terminate the string

      // Process the command
      process_command(received_chars);

      // Reset the buffer for the next command
      char_pos = 0;
    } else {
      received_chars[char_pos] = chr;
      char_pos++;
      if (char_pos >= sizeof(received_chars) - 1) {
        char_pos = 0;  // Reset if overflow
      }
    }
  }
}

void process_command(const char *cmd) {
  char command_type = cmd[0];
  int angle = atoi(&cmd[1]);

  if (angle >= 0 && angle <= 180) {  // Check if angle is within valid range
    switch (command_type) {
      case 'b': reach_goal(waist, angle); break;
      case 's': reach_goal(shoulder, angle); break;
      case 'e': reach_goal(elbow, angle); break;
      case 'w': reach_goal(wrist, angle); break;
      case 't': reach_goal(wrist_twist, angle); break;
      case 'g': reach_goal(tool_tip, angle); break;
      default: break;  // Invalid command type, do nothing
    }
  }
}

void reach_goal(Servo& motor, int goal) {
  int currentPos = motor.read();
  if (goal >= currentPos) {
    for (int pos = currentPos; pos <= goal; pos += 1) {
      motor.write(pos);
      delayMicroseconds(1);
    }
  } else {
    for (int pos = currentPos; pos >= goal; pos -= 1) {
      motor.write(pos);
      delayMicroseconds(1);
    }
  }
}
