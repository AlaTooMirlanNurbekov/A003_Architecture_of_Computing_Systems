/*
  Task 0020 : precise 90° turn using wheel encoders
  Description: this task makes the robot perform an accurate 90-degree turn
  using encoder feedback from both wheels.

  One wheel moves forward, the other moves backward.
  The robot stops when the required encoder pulses are reached.
*/

const int LEFT_ENCODER  = 2; 
const int RIGHT_ENCODER = 3; 

volatile long leftPulses  = 0;
volatile long rightPulses = 0;
const int IN1 = 4;  //left 
const int IN2 = 5;
const int IN3 = 6;  // right 
const int IN4 = 7;

const int ENA = 9;
const int ENB = 10;
const int PULSES_PER_REV = 20;       
const float WHEEL_DIAMETER_CM = 6.5;  
const float WHEEL_BASE_CM = 14.0;    
const float PI_VAL = 3.14159;

const int MOTOR_SPEED = 140;
bool turnDone = false;
long targetPulses = 0;

void leftEncoderISR() {
  leftPulses++;
}

void rightEncoderISR() {
  rightPulses++;
}
void setup() {
  pinMode(LEFT_ENCODER, INPUT_PULLUP);
  pinMode(RIGHT_ENCODER, INPUT_PULLUP);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER), leftEncoderISR, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER), rightEncoderISR, RISING);
  Serial.begin(9600);
  Serial.println("Task 0020: Precise 90-degree turn started");
  float wheelCircumference = PI_VAL * WHEEL_DIAMETER_CM;
  float turnDistance = (PI_VAL * WHEEL_BASE_CM) / 4.0; // 90 degrees
  float revolutionsNeeded = turnDistance / wheelCircumference;
  targetPulses = (long)(revolutionsNeeded * PULSES_PER_REV);

  Serial.print("Target pulses per wheel: ");
  Serial.println(targetPulses);

  digitalWrite(IN1, HIGH); //left wheel forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  //right wheel backward
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, MOTOR_SPEED);
  analogWrite(ENB, MOTOR_SPEED);
}
void loop() {
  if (!turnDone) {
    long l, r;
    noInterrupts();
    l = leftPulses;
    r = rightPulses;
    interrupts();

    if (l >= targetPulses && r >= targetPulses) {
      analogWrite(ENA, 0);
      analogWrite(ENB, 0);
      turnDone = true;
      Serial.println("90-degree turn completed.");
      Serial.print("Left pulses: ");
      Serial.print(l);
      Serial.print(" | Right pulses: ");
      Serial.println(r);
    }
  }
}