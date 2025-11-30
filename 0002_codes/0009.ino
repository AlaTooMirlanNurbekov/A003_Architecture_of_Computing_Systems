/*
  Task 0009 : two-joystick robot arm control (improved version)

  Description : This is a more refined version of the previous robot arm control (Task 0008).
  The idea is still the same, where two joysticks control four servos (base, shoulder,
  elbow and gripper). The difference here is that the movement is smoother and
  a bit more stable. Small joystick noise won’t shake the servos anymore, and
  there is a button that “locks” the arm in place when you release it.

  Joystick → Servo map:
    - J1 X → Base
    - J1 Y → Shoulder
    - J2 X → Elbow
    - J2 Y → Gripper / wrist
*/

#include <Servo.h>

//servo objects
Servo sBase;
Servo sShoulder;
Servo sElbow;
Servo sGripper;

//joystick analog pins
const int joy1X = A0;
const int joy1Y = A1;
const int joy2X = A2;
const int joy2Y = A3;

//pins for servo
const int pinBase     = 3;
const int pinShoulder = 5;
const int pinElbow    = 6;
const int pinGripper  = 9;

//button to enable movement (active LOW)
const int enableButtonPin = 2;

//Joystick calibration values
const int JOY_CENTER   = 512;   // typical center of analog joystick
const int JOY_DEADZONE = 60;    // small area where we ignore movement

//smoothed values (start from center)
int smoothX1 = JOY_CENTER;
int smoothY1 = JOY_CENTER;
int smoothX2 = JOY_CENTER;
int smoothY2 = JOY_CENTER;

// to remember the last servo angles (used when the arm is "held")
int lastBaseAngle     = 90;
int lastShoulderAngle = 90;
int lastElbowAngle    = 90;
int lastGripperAngle  = 90;


// Converts joystick reading to servo angle.
// Joystick near center → servo stays at ~90°
// Full joystick movement → servo goes 10°..170°
int joystickToAngle(int value, int center, int deadZone) {

  // Ignore small movements around center
  if (value > center - deadZone && value < center + deadZone) {
    return 90;
  }

  int angle;

  // Joystick moved to the left/up
  if (value < center - deadZone) {
    angle = map(value, 0, center - deadZone, 10, 90);
  }
  // Joystick moved to the right/down
  else {
    angle = map(value, center + deadZone, 1023, 90, 170);
  }

  return constrain(angle, 10, 170);
}


// Simple smoothing to avoid sudden jumps.
// (Just blends old and new values)
int smooth(int previous, int current) {
  return (previous * 3 + current) / 4;
}


void setup() {
  sBase.attach(pinBase);
  sShoulder.attach(pinShoulder);
  sElbow.attach(pinElbow);
  sGripper.attach(pinGripper);

  pinMode(enableButtonPin, INPUT_PULLUP);

  // Start from a neutral pose
  sBase.write(lastBaseAngle);
  sShoulder.write(lastShoulderAngle);
  sElbow.write(lastElbowAngle);
  sGripper.write(lastGripperAngle);
}


void loop() {
  // Button pressed = movement enabled
  bool enabled = (digitalRead(enableButtonPin) == LOW);

  // Read joysticks
  int rawX1 = analogRead(joy1X);
  int rawY1 = analogRead(joy1Y);
  int rawX2 = analogRead(joy2X);
  int rawY2 = analogRead(joy2Y);

  // Apply smoothing (helps remove joystick noise)
  smoothX1 = smooth(smoothX1, rawX1);
  smoothY1 = smooth(smoothY1, rawY1);
  smoothX2 = smooth(smoothX2, rawX2);
  smoothY2 = smooth(smoothY2, rawY2);

  // Only update when button is pressed
  if (enabled) {
    lastBaseAngle     = joystickToAngle(smoothX1, JOY_CENTER, JOY_DEADZONE);
    lastShoulderAngle = joystickToAngle(smoothY1, JOY_CENTER, JOY_DEADZONE);
    lastElbowAngle    = joystickToAngle(smoothX2, JOY_CENTER, JOY_DEADZONE);
    lastGripperAngle  = joystickToAngle(smoothY2, JOY_CENTER, JOY_DEADZONE);
  }

  //write angles (either updated or held)
  sBase.write(lastBaseAngle);
  sShoulder.write(lastShoulderAngle);
  sElbow.write(lastElbowAngle);
  sGripper.write(lastGripperAngle);

  delay(15);
}
