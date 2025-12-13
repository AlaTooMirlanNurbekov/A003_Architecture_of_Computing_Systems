/*
  Task 0017 : line-following robot with ultrasonic safety stop

  Description:
  This robot follows a line using 3 IR sensors.
  An ultrasonic sensor acts as a safety layer:
  if an obstacle is detected too close, the robot stops
  regardless of line-following logic.

  Priority:
    1. Safety (Ultrasonic)
    2. Navigation (Line following)
*/
const int IR_LEFT   = 6;
const int IR_CENTER = 7;
const int IR_RIGHT  = 8;

const int TRIG_PIN = 12;
const int ECHO_PIN = 11;

const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;
const int ENA = 9;
const int ENB = 10;

const int OBSTACLE_STOP_CM = 15;

const int SPEED_FAST = 160;
const int SPEED_SLOW = 110;

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, SPEED_FAST);
  analogWrite(ENB, SPEED_FAST);
}

void slightLeft() {
  analogWrite(ENA, SPEED_SLOW);
  analogWrite(ENB, SPEED_FAST);
}

void slightRight() {
  analogWrite(ENA, SPEED_FAST);
  analogWrite(ENB, SPEED_SLOW);
}

void hardLeft() {
  analogWrite(ENA, 0);
  analogWrite(ENB, SPEED_FAST);
}

void hardRight() {
  analogWrite(ENA, SPEED_FAST);
  analogWrite(ENB, 0);
}

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

void setup() {
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_CENTER, INPUT);
  pinMode(IR_RIGHT, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
  Serial.println("Task 0017: Line follower with safety stop started");
}


void loop() {
  int distance = measureDistanceCm();

  if (distance < OBSTACLE_STOP_CM) {
    stopMotors();
    Serial.println("SAFETY STOP");
    delay(50);
    return; // skip navigation logic
  }

  // NAVIGATION LAYER
  int L = digitalRead(IR_LEFT);
  int C = digitalRead(IR_CENTER);
  int R = digitalRead(IR_RIGHT);

  if (C == LOW && L == HIGH && R == HIGH) {
    moveForward();
  }
  else if (C == LOW && L == LOW) {
    slightLeft();
  }
  else if (C == LOW && R == LOW) {
    slightRight();
  }
  else if (L == LOW) {
    hardLeft();
  }
  else if (R == LOW) {
    hardRight();
  }
  else {
    stopMotors(); // line lost
  }

  delay(5);
}