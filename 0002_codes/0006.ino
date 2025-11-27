/* 
  Task 0006 : joystick direction LEDs
  Description: Use one joystick module and four LEDs to show the direction 
  of movement. When the joystick is in the center (idle), all LEDs are off. 
  Moving the stick left, right, up or down will turn on one LED for each 
  direction:

  - X left  -> LED1
  - X right -> LED2
  - Y up    -> LED3
  - Y down  -> LED4
*/

const int joyX = A0;   // joystick X axis
const int joyY = A1;   // joystick Y axis

const int ledLeft  = 4;
const int ledRight = 5;
const int ledUp    = 6;
const int ledDown  = 7;

// center value is around 512, we use a small dead zone
const int lowLimit  = 400;
const int highLimit = 600;

void setup() {
  pinMode(ledLeft,  OUTPUT);
  pinMode(ledRight, OUTPUT);
  pinMode(ledUp,    OUTPUT);
  pinMode(ledDown,  OUTPUT);
}

void loop() {
  int x = analogRead(joyX);
  int y = analogRead(joyY);

  // default: all LEDs off
  digitalWrite(ledLeft,  LOW);
  digitalWrite(ledRight, LOW);
  digitalWrite(ledUp,    LOW);
  digitalWrite(ledDown,  LOW);

  // check X axis
  if (x < lowLimit) {
    digitalWrite(ledLeft, HIGH);
  } else if (x > highLimit) {
    digitalWrite(ledRight, HIGH);
  }

  // check Y axis
  if (y < lowLimit) {
    digitalWrite(ledUp, HIGH);
  } else if (y > highLimit) {
    digitalWrite(ledDown, HIGH);
  }

  delay(50);
}
