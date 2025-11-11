/* 
  Task 0002 : Traffic Light Imitation
  Description: In this activity, three LEDs (red, yellow, and green) are connected to the Arduino
  to simulate a basic traffic light system. The sequence should operate as follows:
  - Green light turns ON for 5 seconds.
  - Yellow light turns ON for 2 seconds (green turns OFF).
  - Red light turns ON for 5 seconds (yellow turns OFF).
  The cycle repeats continuously, imitating a simple intersection traffic light.
*/

const int redLed = 11;      // Red LED connected to pin 11
const int yellowLed = 12;   // Yellow LED connected to pin 12
const int greenLed = 13;    // Green LED connected to pin 13

void setup() {
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
}

void loop() {
  // Green ON for 5 seconds
  digitalWrite(greenLed, HIGH);
  digitalWrite(yellowLed, LOW);
  digitalWrite(redLed, LOW);
  delay(5000);

  // Yellow ON for 2 seconds
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, HIGH);
  digitalWrite(redLed, LOW);
  delay(2000);

  // Red ON for 5 seconds
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(redLed, HIGH);
  delay(5000);
}
