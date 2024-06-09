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

uint8_t idx = 0;
uint8_t val_idx = 0;
char value[4] = "000";

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
  if (Serial.available()) {
    char chr = Serial.read();

    // Print the received character to the serial monitor
    Serial.print(chr);

    if (chr == 'b') {
      idx = 0;
      val_idx = 0;
      resetValueBuffer();
    }
    else if (chr == 's') {
      idx = 1;
      val_idx = 0;
      resetValueBuffer();
    }
    else if (chr == 'e') {
      idx = 2;
      val_idx = 0;
      resetValueBuffer();
    }
    else if (chr == 'w') {
      idx = 3;
      val_idx = 0;
      resetValueBuffer();
    }
    else if (chr == 't') {
      idx = 4;
      val_idx = 0;
      resetValueBuffer();
    }
    else if (chr == 'g') {
      idx = 5;
      val_idx = 0;
      resetValueBuffer();
    }
    else if (chr == ',') {
      Serial.print('\n');
      int val = atoi(value);
      if (val >= 0 && val <= 180) {  // Check if value is within valid range
        switch (idx) {
          case 0: reach_goal(waist, val); break;
          case 1: reach_goal(shoulder, val); break;
          case 2: reach_goal(elbow, val); break;
          case 3: reach_goal(wrist, val); break;
          case 4: reach_goal(wrist_twist, val); break;
          case 5: reach_goal(tool_tip, val); break;
        }
      } else {
        Serial.println("Invalid value. Value should be between 0 and 180.");
      }

      // reset the value buffer for the next command
      resetValueBuffer();
      val_idx = 0;
    }
    // Plain number
    else {
      if (val_idx < 3) {  // Ensure val_idx does not overflow
        value[val_idx] = chr;
        val_idx++;
      }
    }
  }
}

void resetValueBuffer() {
  value[0] = '0';
  value[1] = '0';
  value[2] = '0';
  value[3] = '\0';
}

void reach_goal(Servo& motor, int goal) {
  int currentPos = motor.read();
  if (goal >= currentPos) {
    // goes from the start point degrees to the end point degrees
    for (int pos = currentPos; pos <= goal; pos += 1) { 
      motor.write(pos);     
      delayMicroseconds(100);                       
    }
  } else {
    // goes from the end point degrees to the start point degrees
    for (int pos = currentPos; pos >= goal; pos -= 1) { 
      motor.write(pos);     
      delayMicroseconds(100);                       
    }
  }
}
