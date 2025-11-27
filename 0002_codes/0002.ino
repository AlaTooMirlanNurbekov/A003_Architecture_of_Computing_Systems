/* 
  Task 0002 : brightness control LED (PWM)
  Description: A push button is used to change the brightness of an LED.
  Each button press increases the brightness step by step. When the LED 
  reaches the highest level, the next press resets it back to zero. 
*/

const int led = 5;   // LED on a PWM pin
const int btn = 2;   // btn to GND, use internal pull-up

int level = 0;       // 0..4 steps (0%, 25%, 50%, 75%, 100%)

void setup() {
  pinMode(led, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
}

void loop() {

  if (digitalRead(btn) == LOW) {
    while (digitalRead(btn) == LOW) {}   // wait release

    level++;
    if (level > 4) level = 0;

    int brightness = map(level, 0, 4, 0, 255);
    analogWrite(led, brightness);

    delay(150); // simple debounce
  }
}

