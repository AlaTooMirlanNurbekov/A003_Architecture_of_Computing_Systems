/*
  Task 0014 : autonomous obstacle avoiding robot

  Description: this task implements a simple autonomous robot:
  - The robot moves forward when the path is clear.
  - It slows down when an obstacle is getting closer.
  - It stops and turns when an obstacle is too close.
  Core robotics idea:
    Sense (ultrasonic)
      -> Decide (if distance < threshold)
        -> Act (motors)
  Hardware:
    HC-SR04:
      TRIG -> D8
      ECHO -> D7
    L298N:
      IN1 -> D2
      IN2 -> D3
      IN3 -> D4
      IN4 -> D5
      ENA -> D9 (PWM)
      ENB -> D10 (PWM)
*/

// -------- Ultrasonic --------
const int TRIG_PIN = 8;
const int ECHO_PIN = 7;

// -------- Motor driver --------
const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;

const int ENA = 9;   // Left motor speed (PWM)
const int ENB = 10;  // Right motor speed (PWM)

// -------- Distance thresholds (cm) --------
const int SAFE_DIST = 30;
const int SLOW_DIST = 15;

// -------- Motor speeds --------
const int SPEED_FAST = 180;
const int SPEED_SLOW = 120;

// -------- Functions --------

int measureDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration == 0) return 999;

  return duration / 58;
}

void moveForward(int speedVal) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void turnRight() {
  // Left motor forward, right motor backward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, SPEED_SLOW);
  analogWrite(ENB, SPEED_SLOW);
}

//Setup 

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
  Serial.println("Task 0014: Obstacle Avoiding Robot started");
}

void loop() {
  int distance = measureDistanceCm();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > SAFE_DIST) {
    //path is clear
    moveForward(SPEED_FAST);
  }
  else if (distance > SLOW_DIST) {
    // Getting closer
    moveForward(SPEED_SLOW);
  }
  else {
    // Obstacle very close
    stopMotors();
    delay(200);

    turnRight();
    delay(400);

    stopMotors();
    delay(100);
  }

  delay(50); //small loop delay
}
