/* 
  Task 0004: servo barrier gate
  Description: In this task we use a small servo motor and a push button to imitate 
  a simple barrier gate. When the button is pressed, the gate opens 
  slowly, stays open for a short moment, and then closes back.  
  The LED shows that the gate is moving.  
*/

#include <Servo.h>

Servo gate;

const int sPin = 9;
const int led  = 13;
const int btn  = 2;

void setup() {
  gate.attach(sPin);
  pinMode(led, OUTPUT);
  pinMode(btn, INPUT_PULLUP);

  gate.write(0);   // start closed
}

void loop() {

  if (digitalRead(btn) == LOW) {
    while (digitalRead(btn) == LOW) {}   // wait release

    // open
    for (int i = 0; i <= 90; i++) {
      gate.write(i);
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(20);
    }

    delay(3000); // hold open

    // close
    for (int i = 90; i >= 0; i--) {
      gate.write(i);
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(20);
    }
  }
}
