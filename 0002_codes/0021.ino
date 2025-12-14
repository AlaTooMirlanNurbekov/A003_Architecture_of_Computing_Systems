/*
  Task 0021 : drive a square path

  Description: this task is a clean capstone for encoder-based control.
  The robot drives in a square:
    - Move forward a fixed distance (e.g., 50 cm)
    - Turn right 90°
    - Repeat 4 times

  What you practice:
    - Encoder-based distance control (straight motion)
    - Encoder-based angle control (90° turn)
    - Sequencing multiple actions (simple state machine)
    - Debugging real-world errors (slip, wheel mismatch)
  Hardware:
    Encoders (2x):
      LEFT_ENCODER  -> D2 (interrupt)
      RIGHT_ENCODER -> D3 (interrupt)

    L298N Motor Driver:
      Left motor:  IN1 -> D4, IN2 -> D5, ENA -> D9  (PWM)
      Right motor: IN3 -> D6, IN4 -> D7, ENB -> D10 (PWM)
*/

#include <Arduino.h>


const int LEFT_ENCODER  = 2; // interrupt pin
const int RIGHT_ENCODER = 3; // interrupt pin
volatile long leftPulses  = 0;
volatile long rightPulses = 0;
const int IN1 = 4;  //left motor direction
const int IN2 = 5;
const int IN3 = 6;  // right motor direction
const int IN4 = 7;

const int ENA = 9;   // left motor PWM
const int ENB = 10;  // right motor PWM

const int   PULSES_PER_REV     = 20;    // encoder resolution
const float WHEEL_DIAMETER_CM  = 6.5;   // measure your wheel
const float WHEEL_BASE_CM      = 14.0;  // distance between wheels
const float PI_VAL             = 3.14159;
const float SIDE_LENGTH_CM = 50.0;  // square side distance
const int SPEED_FORWARD    = 150;
const int SPEED_TURN       = 140;

int sideCount = 0;

// Simple state machine
enum State {
  MOVE_FORWARD,
  TURN_RIGHT,
  DONE
};

State state = MOVE_FORWARD;
long targetPulses = 0;

void leftEncoderISR()  { leftPulses++;  }
void rightEncoderISR() { rightPulses++; }

void resetEncoders() {
  noInterrupts();
  leftPulses = 0;
  rightPulses = 0;
  interrupts();
}

long pulsesForDistanceCm(float distanceCm) {
  float wheelCirc = PI_VAL * WHEEL_DIAMETER_CM;         
  float revs = distanceCm / wheelCirc;                 
  return (long)(revs * PULSES_PER_REV);
}

long pulsesForRightTurn90() {
  float turnDistance = (PI_VAL * WHEEL_BASE_CM) / 4.0;
  return pulsesForDistanceCm(turnDistance);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void driveForward(int speedVal) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

void turnRightInPlace(int speedVal) {
  // left forward, right backward
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

bool reachedTarget(long l, long r, long target) {
  return (l >= target && r >= target);
}

void setup() {
  pinMode(LEFT_ENCODER, INPUT_PULLUP);
  pinMode(RIGHT_ENCODER, INPUT_PULLUP);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER),  leftEncoderISR,  RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER), rightEncoderISR, RISING);

  Serial.begin(9600);
  Serial.println("Task 0021: Drive a square path started");

  // move forward one side
  resetEncoders();
  targetPulses = pulsesForDistanceCm(SIDE_LENGTH_CM);
  driveForward(SPEED_FORWARD);

  Serial.print("Target pulses (forward): ");
  Serial.println(targetPulses);
}

void loop() {
  if (state == DONE) return;

  long l, r;
  noInterrupts();
  l = leftPulses;
  r = rightPulses;
  interrupts();

  if (state == MOVE_FORWARD) {
    if (reachedTarget(l, r, targetPulses)) {
      stopMotors();
      delay(150);
      //90° turn
      state = TURN_RIGHT;
      resetEncoders();
      targetPulses = pulsesForRightTurn90();
      turnRightInPlace(SPEED_TURN);
      Serial.print("Side ");
      Serial.print(sideCount + 1);
      Serial.println(" done -> turning right");
      Serial.print("Target pulses (turn): ");
      Serial.println(targetPulses);
    }
  }
  else if (state == TURN_RIGHT) {
    if (reachedTarget(l, r, targetPulses)) {
      stopMotors();
      delay(150);
      sideCount++;
      if (sideCount >= 4) {
        state = DONE;
        stopMotors();
        Serial.println("Square completed. DONE.");
      } else {
        // move forward again
        state = MOVE_FORWARD;
        resetEncoders();
        targetPulses = pulsesForDistanceCm(SIDE_LENGTH_CM);
        driveForward(SPEED_FORWARD);
        Serial.print("Turn complete -> starting side ");
        Serial.println(sideCount + 1);
        Serial.print("Target pulses (forward): ");
        Serial.println(targetPulses);
      }
    }
  }
}
