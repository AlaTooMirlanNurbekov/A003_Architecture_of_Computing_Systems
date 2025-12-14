/*
  Task 0022 : basic odometry using wheel encoders

  Description:
  This task estimates the robot's position and heading using odometry.
  The robot moves forward for a fixed time and prints:
    - x position (cm)
    - y position (cm)
    - heading theta (degrees)

  Key idea:
    Left and right wheel distances tell you:
      - how far the robot moved (center distance)
      - how much it rotated (change in heading)

  This is a foundation for:
    - navigation
    - mapping
    - path planning

  Hardware:
    Encoders (2x):
      LEFT_ENCODER  -> D2 (interrupt)
      RIGHT_ENCODER -> D3 (interrupt)

    L298N Motor Driver:
      Left:  IN1 -> D4, IN2 -> D5, ENA -> D9
      Right: IN3 -> D6, IN4 -> D7, ENB -> D10
*/

#include <Arduino.h>
const int LEFT_ENCODER  = 2;
const int RIGHT_ENCODER = 3;
volatile long leftPulses  = 0;
volatile long rightPulses = 0;
const int IN1 = 4;
const int IN2 = 5;
const int IN3 = 6;
const int IN4 = 7;
const int ENA = 9;
const int ENB = 10;

const int   PULSES_PER_REV    = 20;
const float WHEEL_DIAMETER_CM = 6.5;
const float WHEEL_BASE_CM     = 14.0;   // distance between wheels
const float PI_VAL            = 3.14159;

const int SPEED_FWD = 150;

float x_cm = 0.0;
float y_cm = 0.0;
float theta_rad = 0.0; 

unsigned long lastUpdateMs = 0;
const unsigned long UPDATE_INTERVAL_MS = 200;

long prevLeft = 0;
long prevRight = 0;
void leftEncoderISR()  { leftPulses++;  }
void rightEncoderISR() { rightPulses++; }

float pulsesToDistanceCm(long pulses) {
  float wheelCirc = PI_VAL * WHEEL_DIAMETER_CM;
  float revs = (float)pulses / (float)PULSES_PER_REV;
  return revs * wheelCirc;
}

void driveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, SPEED_FWD);
  analogWrite(ENB, SPEED_FWD);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

float normalizeAngle(float a) {
  while (a > PI_VAL)  a -= 2.0 * PI_VAL;
  while (a < -PI_VAL) a += 2.0 * PI_VAL;
  return a;
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
  Serial.println("Task 0022: Basic Odometry started");
  Serial.println("x_cm,y_cm,theta_deg");

  driveForward();
  lastUpdateMs = millis();
}

void loop() {
  unsigned long now = millis();

  // run for ~8 sec
  if (now > 8000) {
    stopMotors();
  }

  if (now - lastUpdateMs >= UPDATE_INTERVAL_MS) {
    lastUpdateMs = now;

    long l, r;
    noInterrupts();
    l = leftPulses;
    r = rightPulses;
    interrupts();
    long dL_p = l - prevLeft;
    long dR_p = r - prevRight;
    prevLeft = l;
    prevRight = r;

    float dL = pulsesToDistanceCm(dL_p);
    float dR = pulsesToDistanceCm(dR_p);

    float dCenter = (dL + dR) / 2.0;
    float dTheta  = (dR - dL) / WHEEL_BASE_CM;  // radians


    float thetaMid = theta_rad + dTheta / 2.0;
    x_cm += dCenter * cos(thetaMid);
    y_cm += dCenter * sin(thetaMid);
    theta_rad = normalizeAngle(theta_rad + dTheta);

    float theta_deg = theta_rad * 180.0 / PI_VAL;
    Serial.print(x_cm, 2);
    Serial.print(",");
    Serial.print(y_cm, 2);
    Serial.print(",");
    Serial.println(theta_deg, 1);
  }
}
