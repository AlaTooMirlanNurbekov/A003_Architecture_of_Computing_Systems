/*
  Task 0015 : line following robot

  Description: this task implements a simple two-sensor line-following robot.

  - Two IR reflective sensors detect a black line on a white surface.
  - Based on sensor readings, the robot adjusts motor direction.
  - This demonstrates discrete control logic used in real robots.

  Sensor logic (common modules):
    LOW  -> black surface
    HIGH -> white surface
*/

//IR Sensors
const int LEFT_IR  = 6;
const int RIGHT_IR = 7;

//motor driver
const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;

const int ENA = 9;   // Left motor speed
const int ENB = 10;  // Right motor speed

// speeds
const int SPEED_BASE = 150;
const int SPEED_TURN = 120;

//motor control functions 
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, SPEED_BASE);
  analogWrite(ENB, SPEED_BASE);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);      //left motor stop
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);      // right motor forward

  analogWrite(ENA, 0);
  analogWrite(ENB, SPEED_TURN);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);      // left motor forward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);      //right motor stop

  analogWrite(ENA, SPEED_TURN);
  analogWrite(ENB, 0);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void setup() {
  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
  Serial.println("Task 0015: Line-Following Robot started");
}

void loop() {
  int leftState  = digitalRead(LEFT_IR);
  int rightState = digitalRead(RIGHT_IR);

  Serial.print("L:");
  Serial.print(leftState);
  Serial.print(" R:");
  Serial.println(rightState);

  //both sensors on black line
  if (leftState == LOW && rightState == LOW) {
    moveForward();
  }
  // Left sees line, right sees white
  else if (leftState == LOW && rightState == HIGH) {
    turnLeft();
  }
  // Right sees line, left sees white
  else if (leftState == HIGH && rightState == LOW) {
    turnRight();
  }
  //Line lost
  else {
    stopMotors();
  }

  delay(10); // small stability delay
}
