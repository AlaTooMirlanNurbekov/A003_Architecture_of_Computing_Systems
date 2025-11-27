/* 
  Task 0005 : servo angle with potentiometer
  Description: This task uses a potentiometer to control the angle of a small servo motor.
  When you turn the knob, the servo position changes from 0° to 180°. 
*/

#include <Servo.h>

Servo arm; // creating an object named arm

const int sPin = 9;    // servo signal
const int pot  = A0;   // potentiometer

void setup() {
  arm.attach(sPin);
}

void loop() {
  int val = analogRead(pot);          // 0..1023
  int angle = map(val, 0, 1023, 0, 180);
  arm.write(angle);
  delay(15);
}
