// Task 0010 : bluetooth control for an LED
// Description: Receives ON/OFF/STATE commands from phone
// and sends back the LED state

int ledPin = 13;
bool ledState = false; // start with LED off
String inputData = ""; // incoming command holder

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(9600); 
  // note: HC-05/06 must be set to the same baud

  // Let the phone know the module is alive
  Serial.println("CONNECTED");
  sendLedState();
}

void loop() {
  // Check if phone sent anything
  if (Serial.available()) {
    char c = Serial.read();

    // accumulate until newline
    if (c == '\n') {
      processCommand(inputData);
      inputData = "";
    } else {
      inputData += c;
    }
  }
}

// handle text commands from the phone
void processCommand(String cmd) {

  cmd.trim(); // remove spaces, CR, etc.

  if (cmd == "ON") {
    ledState = true;
    digitalWrite(ledPin, HIGH);
    sendLedState();
  }
  else if (cmd == "OFF") {
    ledState = false;
    digitalWrite(ledPin, LOW);
    sendLedState();
  }
  else if (cmd == "STATE") {
    sendLedState();
  }
  else {
    //any unknown text
    Serial.println("ERR");
  }
}

//  sends the LED status to the phone
void sendLedState() {
  if (ledState) {
    Serial.println("LED:ON");
  } else {
    Serial.println("LED:OFF");
  }
}
