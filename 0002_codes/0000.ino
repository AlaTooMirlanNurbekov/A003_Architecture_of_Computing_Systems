/* 
  Task 0001 : Alternating Dual LED System
  Description: In this activity, two LEDs are connected to the Arduino along with a single push button.
  When the button is pressed, the LEDs should alternate states — one turns ON while the other turns OFF.
  This switching should continue 100 times (a full cycle means both LEDs have changed once).
  After completing all cycles, the system should stop and wait until the button is pressed again.
*/

const int led1 = 12;      // First LED connected to pin 12
const int led2 = 13;      // Second LED connected to pin 13
const int buttonPin = 2;  // Button connected to pin 2

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // use internal pull-up resistor
}

void loop() {
  // wait for button press
  if (digitalRead(buttonPin) == LOW) {
    // alternate LEDs 100 times
    for (int i = 0; i < 100; i++) {
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      delay(300);
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      delay(300);
    }

    // turn both off after loop
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);

    // wait until button released
    while (digitalRead(buttonPin) == LOW) {
      delay(10);
    }
  }
}
