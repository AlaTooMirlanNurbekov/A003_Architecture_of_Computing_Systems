/*
  Task 0018 : wheel encoder distance & speed measurement

  Description: this task measures how far a robot wheel has traveled using a wheel encoder.
  The encoder generates pulses as the wheel rotates.
  Using interrupts, the Arduino:
    - counts pulses
    - calculates distance (cm)
    - calculates wheel speed (RPM)
*/


const int ENCODER_PIN = 2;

volatile long pulseCount = 0;
const int IN1 = 4;
const int IN2 = 5;
const int ENA = 9;

const int PULSES_PER_REV = 20;      // encoder slots (adjust to your encoder)
const float WHEEL_DIAMETER_CM = 6.5; // measure your wheel
const float PI_VAL = 3.14159;

unsigned long lastCalcMs = 0;
const unsigned long CALC_INTERVAL_MS = 1000;

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
  Serial.println("Task 0018: Wheel Encoder Measurement Started");

  // motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 150);
}

void loop() {
  unsigned long now = millis();

  if (now - lastCalcMs >= CALC_INTERVAL_MS) {
    lastCalcMs = now;

    // Copy volatile value safely
    noInterrupts();
    long pulses = pulseCount;
    pulseCount = 0;
    interrupts();

    //calculations
    float revolutions = (float)pulses / PULSES_PER_REV;
    float wheelCircumference = PI_VAL * WHEEL_DIAMETER_CM;
    float distanceCm = revolutions * wheelCircumference;
    float rpm = revolutions * 60.0;
    Serial.print("Pulses: ");
    Serial.print(pulses);
    Serial.print(" | Distance (cm): ");
    Serial.print(distanceCm, 2);
    Serial.print(" | Speed (RPM): ");
    Serial.println(rpm, 1);
  }
}
