/*
  Task 0019 : move a precise distance using wheel encoder

  Description: this task moves the robot forward for a precise distance
  using encoder feedback instead of delay-based timing.

  The robot:
    - calculates required encoder pulses for a target distance
    - starts the motor
    - stops automatically when distance is reached
*/

const int ENCODER_PIN = 2; // interrupt pin
volatile long pulseCount = 0;

const int IN1 = 4;
const int IN2 = 5;
const int ENA = 9;

const int PULSES_PER_REV = 20;       // adjust for your encoder
const float WHEEL_DIAMETER_CM = 6.5; // measure your wheel
const float PI_VAL = 3.14159;

const float TARGET_DISTANCE_CM = 50.0;
const int MOTOR_SPEED = 150;

long targetPulses = 0;
bool motionDone = false;

void encoderISR() {
  pulseCount++;
}

void setup() {
  pinMode(ENCODER_PIN, INPUT_PULLUP);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), encoderISR, RISING);

  Serial.begin(9600);
  Serial.println("Task 0019: Precise distance movement started");

  // Calculate pulses needed
  float wheelCircumference = PI_VAL * WHEEL_DIAMETER_CM;
  float revolutionsNeeded = TARGET_DISTANCE_CM / wheelCircumference;
  targetPulses = (long)(revolutionsNeeded * PULSES_PER_REV);

  Serial.print("Target distance (cm): ");
  Serial.println(TARGET_DISTANCE_CM);
  Serial.print("Target pulses: ");
  Serial.println(targetPulses);

  // Start motor
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, MOTOR_SPEED);
}
void loop() {
  if (!motionDone) {
    long pulses;

    //read encoder safely
    noInterrupts();
    pulses = pulseCount;
    interrupts();

    if (pulses >= targetPulses) {
      // Stop motor
      analogWrite(ENA, 0);
      motionDone = true;

      Serial.println("Target distance reached. Motor stopped.");
      Serial.print("Final pulses: ");
      Serial.println(pulses);
    }
  }
}
