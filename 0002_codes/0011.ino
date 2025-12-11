/*
  Task 0011 : bluetooth-controlled servo angle
  Description: In this task you control a servo motor from your phone using Bluetooth.
  The HC-06 / HC-05 module is connected to Arduino with SoftwareSerial.

  You can send commands from a Bluetooth terminal app on your phone:
    "0"   .. "180"   -> set servo to that angle
    "CENTER"        -> move servo to 90 degrees
    "+10"           -> increase angle by 10 degrees
    "-10"           -> decrease angle by 10 degrees

  After each valid command, Arduino replies with the current angle:
    "ANGLE:<value>"

  This task shows how to:
    - Use Bluetooth for serial communication
    - Parse simple text commands
    - Keep and update servo state safely (with angle limits)
*/
#include <SoftwareSerial.h>
#include <Servo.h>

// Bluetooth pins (Arduino side)
const int BT_RX = 11; // Arduino RX (to BT TX)
const int BT_TX = 10; // Arduino TX (to BT RX)

// Servo pin
const int SERVO_PIN = 9;

//angle limits
const int MIN_ANGLE = 0;
const int MAX_ANGLE = 180;

SoftwareSerial bt(BT_RX, BT_TX);
Servo servo;

int currentAngle = 90;  // start from center position

String command = "";

// clamp an angle to valid range
int clampAngle(int angle) {
  if (angle < MIN_ANGLE) return MIN_ANGLE;
  if (angle > MAX_ANGLE) return MAX_ANGLE;
  return angle;
}

// Move servo and update currentAngle
void setServoAngle(int angle) {
  angle = clampAngle(angle);
  currentAngle = angle;
  servo.write(currentAngle);
}

// Send current angle to Bluetooth and Serial
void sendAngle() {
  bt.print("ANGLE:");
  bt.println(currentAngle);

  Serial.print("ANGLE:");
  Serial.println(currentAngle);
}

// Try to interpret the command and act
void handleCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();

  Serial.print("Received command: ");
  Serial.println(cmd);

  // CENTER command
  if (cmd == "CENTER") {
    setServoAngle(90);
    sendAngle();
    return;
  }
  // Relative angle commands like +10 or -15
  if ((cmd.startsWith("+") || cmd.startsWith("-")) && cmd.length() > 1) {
    int delta = cmd.substring(1).toInt(); // read number after + or -

    if (delta == 0) {
      bt.println("ERR:BAD_DELTA");
      Serial.println("Bad delta.");
      return;
    }

    if (cmd.charAt(0) == '-') {
      delta = -delta;
    }

    setServoAngle(currentAngle + delta);
    sendAngle();
    return;
  }

  // If not CENTER or relative, try to treat the command as a number (0–180)
  bool isNumber = true;
  for (unsigned int i = 0; i < cmd.length(); i++) {
    if (!isDigit(cmd.charAt(i))) {
      isNumber = false;
      break;
    }
  }

  if (isNumber && cmd.length() > 0) {
    int angle = cmd.toInt();
    setServoAngle(angle);
    sendAngle();
  } else {
    // Unknown command
    bt.println("ERR:UNKNOWN_CMD");
    bt.println("Use: 0-180, CENTER, +10, -10");
    Serial.println("Unknown command.");
  }
}

void setup() {
  Serial.begin(9600); // USB serial for debugging
  bt.begin(9600);     // HC-06 default baud rate

  servo.attach(SERVO_PIN);
  setServoAngle(currentAngle); // move to initial position

  Serial.println("Task 0011: Bluetooth servo control ready.");
  bt.println("READY:SERVO");
  sendAngle(); // send initial angle
}

void loop() {
  // Read command from Bluetooth line-by-line
  while (bt.available() > 0) {
    char c = bt.read();

    if (c == '\n' || c == '\r') {
      if (command.length() > 0) {
        handleCommand(command);
        command = "";
      }
    } else {
      if (command.length() < 20) {
        command += c;
      }
    }
  }
  // U can forward Serial input to Bluetooth if you want
  // for debugging using Serial Monitor.
  if (Serial.available() > 0) {
    char c = Serial.read();
    bt.write(c);
  }
}
