/*
  Task 0016 : 3-sensor line-following robot

  Description: this task improves the classic line-following robot by using
  three IR sensors (LEFT, CENTER, RIGHT).

  The center sensor provides a reference, making the robot
  more stable and realistic compared to a 2-sensor design.

  Sensor logic:
    LOW  -> black line
    HIGH -> white surface
*/

//IR Sensors
const int IR_LEFT   = 6;
const int IR_CENTER = 7;
const int IR_RIGHT  = 8;

//motor driver
const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;

const int ENA = 9;
const int ENB = 10;

//speed values
const int SPEED_FAST  = 160;
const int SPEED_SLOW  = 110;
const int SPEED_TURN  = 140;

// motor control

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
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
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, SPEED_TURN);
}

void hardRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, SPEED_TURN);
  analogWrite(ENB, 0);
}
void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
void setup() {
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_CENTER, INPUT);
  pinMode(IR_RIGHT, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
  Serial.println("Task 0016: 3-Sensor Line Follower started");
}
void loop() {
  int L = digitalRead(IR_LEFT);
  int C = digitalRead(IR_CENTER);
  int R = digitalRead(IR_RIGHT);

  Serial.print("L:");
  Serial.print(L);
  Serial.print(" C:");
  Serial.print(C);
  Serial.print(" R:");
  Serial.println(R);

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
    stopMotors(); // line was lost
  }

  delay(5); // Stability delay
}
