/*
  Task 0013 : ultrasonic obstacle distance radar

  Description: This task turns an Arduino into a simple "parking sensor" / robot distance radar 
  - HC-SR04 measures distance in centimeters.
  - 4 LEDs show distance level (far -> few LEDs, near -> more LEDs).
  - A buzzer beeps faster when an object is closer.
  - Uses millis() timing (no long delay loops), which is important in robotics.

  Hardware:
    HC-SR04:
      VCC  -> 5V
      GND  -> GND
      TRIG -> D8
      ECHO -> D7

    LEDs
      LED1 -> D2
      LED2 -> D3
      LED3 -> D4
      LED4 -> D5

    Buzzer:
      BUZZ -> D6
*/

const int TRIG_PIN = 8;
const int ECHO_PIN = 7;

const int LED1 = 2;
const int LED2 = 3;
const int LED3 = 4;
const int LED4 = 5;

const int BUZZER = 6;
//distance thresholds (cm) - tune for your classroom setup
const int THRESH_FAR    = 80;  // farther than this -> basically safe
const int THRESH_MID    = 50;
const int THRESH_NEAR   = 25;
const int THRESH_DANGER = 12;  // very close

//timing (millis-based)
unsigned long lastMeasureMs = 0;
const unsigned long MEASURE_INTERVAL_MS = 80; // ~12.5 readings/sec

unsigned long lastBeepToggleMs = 0;
bool buzzerOn = false;

int lastDistanceCm = 999;

// Measure distance using HC-SR04 (returns cm, or 999 if out of range)
int measureDistanceCm() {
  // Trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  //Read echo pulse duration (timeout to avoid freezing)
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL); // 30ms -> ~5m max

  if (duration == 0) return 999; // timeout / no echo

  // Speed of sound approx: 343 m/s => 29.1 us/cm (round trip => /58)
  int cm = (int)(duration / 58UL);
  return cm;
}

//Update LEDs based on distance
void updateLeds(int cm) {
  // Default all off
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);

  if (cm >= THRESH_FAR || cm == 999) {
    // Far / no reading -> 1 LED (optional), or all off
    digitalWrite(LED1, HIGH);
  } else if (cm >= THRESH_MID) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
  } else if (cm >= THRESH_NEAR) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
  } else {
    // Near / danger
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
  }
}

// Decide beep interval based on distance (ms).
// Return 0 for continuous ON, and a very large number for OFF.
unsigned long beepIntervalForDistance(int cm) {
  if (cm == 999) return 0xFFFFFFFF; // no object -> no beep
  if (cm <= THRESH_DANGER) return 0;      // continuous warning
  if (cm <= THRESH_NEAR)   return 120;    // fast beeps
  if (cm <= THRESH_MID)    return 250;    // medium beeps
  if (cm <= THRESH_FAR)    return 500;    // slow beeps
  return 0xFFFFFFFF;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  Serial.begin(9600);
  Serial.println("Task 0013: Ultrasonic Distance Radar started.");
}

void loop() {
  unsigned long now = millis();

  // Measure distance periodically
  if (now - lastMeasureMs >= MEASURE_INTERVAL_MS) {
    lastMeasureMs = now;

    lastDistanceCm = measureDistanceCm();
    updateLeds(lastDistanceCm);

    Serial.print("Distance (cm): ");
    Serial.println(lastDistanceCm);
  }

  // Beeper logic (non-blocking)
  unsigned long interval = beepIntervalForDistance(lastDistanceCm);

  if (interval == 0) {
    // Continuous warning
    digitalWrite(BUZZER, HIGH);
    buzzerOn = true;
  } else if (interval == 0xFFFFFFFF) {
    // Off
    digitalWrite(BUZZER, LOW);
    buzzerOn = false;
  } else {
    //toggle buzzer on/off at chosen interval
    if (now - lastBeepToggleMs >= interval) {
      lastBeepToggleMs = now;
      buzzerOn = !buzzerOn;
      digitalWrite(BUZZER, buzzerOn ? HIGH : LOW);
    }
  }
}
