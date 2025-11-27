/* 
  Task 0008 : two-joystick robot arm control
  Description: This task uses two joystick modules to control four servo motors that 
  represent a simple robot arm. Each axis of the joysticks moves one servo:
  - Joystick 1 X -> base rotation (servo 0001)
  - Joystick 1 Y -> shoulder (servo 0002)
  - Joystick 2 X -> elbow (servo 0003)
  - Joystick 2 Y -> gripper or wrist (servo 0004)
  Moving a joystick changes the angle of the corresponding servo. 
*/

#include <Servo.h>

Servo sBase;
Servo sShoulder;
Servo sElbow;
Servo sGripper;

const int joy1X = A0;
const int joy1Y = A1;
const int joy2X = A2;
const int joy2Y = A3;

const int pinBase     = 3;   // servo 0001
const int pinShoulder = 5;   // servo 0002
const int pinElbow    = 6;   // servo 0003
const int pinGripper  = 9;   // servo 0004

void setup() {
  sBase.attach(pinBase);
  sShoulder.attach(pinShoulder);
  sElbow.attach(pinElbow);
  sGripper.attach(pinGripper);
}

void loop() {
  int x1 = analogRead(joy1X);
  int y1 = analogRead(joy1Y);
  int x2 = analogRead(joy2X);
  int y2 = analogRead(joy2Y);

  int aBase     = map(x1, 0, 1023, 0, 180);
  int aShoulder = map(y1, 0, 1023, 0, 180);
  int aElbow    = map(x2, 0, 1023, 0, 180);
  int aGripper  = map(y2, 0, 1023, 0, 180);

  // small limits to avoid hitting hard ends
  aBase     = constrain(aBase, 10, 170);
  aShoulder = constrain(aShoulder, 10, 170);
  aElbow    = constrain(aElbow, 10, 170);
  aGripper  = constrain(aGripper, 10, 170);

  sBase.write(aBase);
  sShoulder.write(aShoulder);
  sElbow.write(aElbow);
  sGripper.write(aGripper);

  delay(15);
}
