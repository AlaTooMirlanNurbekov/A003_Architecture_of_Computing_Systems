/* 
  Task 0007 : IR-controlled LED blink pattern
  Description: A VS1838B IR receiver and a standard IR remote 
  are used to control an LED. Each numbered button on the remote triggers 
  the LED to blink a specific number of times. For example, button “9” blinks 
  the LED nine times, button “1” blinks once, and so on. After completing the 
  blink sequence, the LED returns to the ON state and waits for the next command.
*/

#include <IRremote.hpp>

#define IR_PIN 11     // IR receiver pin
#define LED_PIN 7     // LED pin

// Command values for the remote buttons
#define BTN_1_CMD 0x0C
#define BTN_2_CMD 0x18
#define BTN_3_CMD 0x5E
#define BTN_4_CMD 0x08
#define BTN_5_CMD 0x1C
#define BTN_6_CMD 0x5A
#define BTN_7_CMD 0x42
#define BTN_8_CMD 0x52
#define BTN_9_CMD 0x4A

// Simple blink function
void blinkLed(int times, int intervalMs) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, LOW);
    delay(intervalMs);
    digitalWrite(LED_PIN, HIGH);
    delay(intervalMs);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);    // LED on by default
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    uint8_t cmd = IrReceiver.decodedIRData.command;

    if (cmd == BTN_1_CMD) blinkLed(1, 200);
    if (cmd == BTN_2_CMD) blinkLed(2, 200);
    if (cmd == BTN_3_CMD) blinkLed(3, 200);
    if (cmd == BTN_4_CMD) blinkLed(4, 200);
    if (cmd == BTN_5_CMD) blinkLed(5, 200);
    if (cmd == BTN_6_CMD) blinkLed(6, 200);
    if (cmd == BTN_7_CMD) blinkLed(7, 200);
    if (cmd == BTN_8_CMD) blinkLed(8, 200);
    if (cmd == BTN_9_CMD) blinkLed(9, 200);

    digitalWrite(LED_PIN, HIGH);  // return LED to ON state
    IrReceiver.resume();
  }
}
