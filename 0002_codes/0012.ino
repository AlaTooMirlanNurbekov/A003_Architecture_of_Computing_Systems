/*
  Task 0012 : bluetooth-controlled robot arm with preset poses

  Description: this task controls a 4-servo robot arm using Bluetooth commands.
  A phone (or laptop) connects to an HC-06 / HC-05 module and sends
  simple text commands:
    "HOME"   -> move arm to neutral position
    "PICK"   -> move arm to "pick" pose
    "PLACE"  -> move arm to "place" pose
    "OPEN"   -> open the gripper
    "CLOSE"  -> close the gripper

  After each valid command, Arduino replies with:
    "POSE:HOME", "POSE:PICK", "POSE:PLACE"
  or
    "GRIPPER:OPEN", "GRIPPER:CLOSE"

  This combines:
    - Multiple servos (base, shoulder, elbow, gripper)
    - Bluetooth communication (SoftwareSerial)
    - Smooth movement between poses
*/
#include <SoftwareSerial.h>
#include <Servo.h>
// ---------------- Bluetooth setup ----------------
const int BT_RX = 11; // Arduino RX  (connect to HC-06 TX)
const int BT_TX = 12; // Arduino TX  (connect to HC-06 RX, via divider)

SoftwareSerial bt(BT_RX, BT_TX);

// ---------------- Servo setup --------------------
const int PIN_BASE     = 5;
const int PIN_SHOULDER = 6;
const int PIN_ELBOW    = 9;
const int PIN_GRIPPER  = 10;

Servo sBase;
Servo sShoulder;
Servo sElbow;
Servo sGripper;
// Current angles
int baseAngle     = 90;
int shoulderAngle = 90;
int elbowAngle    = 90;
int gripperAngle  = 90;

//Preset poses
// HOME pose
const int HOME_BASE     = 90;
const int HOME_SHOULDER = 90;
const int HOME_ELBOW    = 90;
const int HOME_GRIPPER  = 90;

// PICK pose (for example: arm down and forward)
const int PICK_BASE     = 90;
const int PICK_SHOULDER = 120;
const int PICK_ELBOW    = 60;
const int PICK_GRIPPER  = 60;  // maybe slightly closed

// PLACE pose (for example: arm a bit to side)
const int PLACE_BASE     = 120;
const int PLACE_SHOULDER = 100;
const int PLACE_ELBOW    = 70;
const int PLACE_GRIPPER  = 60;

// Gripper open/close angles (override GRIPPER in poses if needed)
const int GRIPPER_OPEN  = 20;
const int GRIPPER_CLOSE = 80;

// Command buffer
String command = "";

//Helper functions

// Clamp angle for safety
int clampAngle(int a) {
  if (a < 0)   return 0;
  if (a > 180) return 180;
  return a;
}

// Write current angles to servos
void writeAllServos() {
  sBase.write(clampAngle(baseAngle));
  sShoulder.write(clampAngle(shoulderAngle));
  sElbow.write(clampAngle(elbowAngle));
  sGripper.write(clampAngle(gripperAngle));
}

// Smooth movement from current pose to target pose
void moveToPose(int targetBase, int targetShoulder, int targetElbow, int targetGripper) {
  // Make sure targets are safe
  targetBase     = clampAngle(targetBase);
  targetShoulder = clampAngle(targetShoulder);
  targetElbow    = clampAngle(targetElbow);
  targetGripper  = clampAngle(targetGripper);

  // Decide how many small steps to use
  const int steps = 40;       // more steps = smoother, but slower
  const int delayMs = 20;     // delay between steps

  int startBase     = baseAngle;
  int startShoulder = shoulderAngle;
  int startElbow    = elbowAngle;
  int startGripper  = gripperAngle;

  for (int i = 1; i <= steps; i++) {
    float t = (float)i / (float)steps;

    baseAngle     = startBase     + (int)((targetBase     - startBase)     * t);
    shoulderAngle = startShoulder + (int)((targetShoulder - startShoulder) * t);
    elbowAngle    = startElbow    + (int)((targetElbow    - startElbow)    * t);
    gripperAngle  = startGripper  + (int)((targetGripper  - startGripper)  * t);

    writeAllServos();
    delay(delayMs);
  }

  // Ensure exact final values
  baseAngle     = targetBase;
  shoulderAngle = targetShoulder;
  elbowAngle    = targetElbow;
  gripperAngle  = targetGripper;
  writeAllServos();
}
// Send a short status message
void sendStatus(const String &msg) {
  bt.println(msg);
  Serial.println(msg);
}

// Process received command
void handleCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();

  Serial.print("Received: ");
  Serial.println(cmd);

  if (cmd == "HOME") {
    moveToPose(HOME_BASE, HOME_SHOULDER, HOME_ELBOW, HOME_GRIPPER);
    sendStatus("POSE:HOME");
  }
  else if (cmd == "PICK") {
    moveToPose(PICK_BASE, PICK_SHOULDER, PICK_ELBOW, PICK_GRIPPER);
    sendStatus("POSE:PICK");
  }
  else if (cmd == "PLACE") {
    moveToPose(PLACE_BASE, PLACE_SHOULDER, PLACE_ELBOW, PLACE_GRIPPER);
    sendStatus("POSE:PLACE");
  }
  else if (cmd == "OPEN") {
    gripperAngle = GRIPPER_OPEN;
    sGripper.write(clampAngle(gripperAngle));
    sendStatus("GRIPPER:OPEN");
  }
  else if (cmd == "CLOSE") {
    gripperAngle = GRIPPER_CLOSE;
    sGripper.write(clampAngle(gripperAngle));
    sendStatus("GRIPPER:CLOSE");
  }
  else {
    sendStatus("ERR:UNKNOWN_CMD");
    bt.println("Use: HOME, PICK, PLACE, OPEN, CLOSE");
  }
}

// ----------------- Setup & loop -------------------

void setup() {
  Serial.begin(9600); // for debugging
  bt.begin(9600);     // HC-06 default

  sBase.attach(PIN_BASE);
  sShoulder.attach(PIN_SHOULDER);
  sElbow.attach(PIN_ELBOW);
  sGripper.attach(PIN_GRIPPER);

  // Start in HOME pose
  baseAngle     = HOME_BASE;
  shoulderAngle = HOME_SHOULDER;
  elbowAngle    = HOME_ELBOW;
  gripperAngle  = HOME_GRIPPER;
  writeAllServos();

  sendStatus("Task 0012: Robot arm ready.");
  sendStatus("Commands: HOME, PICK, PLACE, OPEN, CLOSE");
}

void loop() {
  //read bluetooth input line-by-line
  while (bt.available() > 0) {
    char c = bt.read();

    if (c == '\n' || c == '\r') {
      if (command.length() > 0) {
        handleCommand(command);
        command = "";
      }
    } else {
      if (command.length() < 30) {
        command += c;
      }
    }
  }
  // You also can forward Serial Monitor text to Bluetooth
  if (Serial.available() > 0) {
    char c = Serial.read();
    bt.write(c);
  }
}
