/* 
  Task 0003 : button-triggered buzzer melody
  Description: a buzzer and a push button are connected to the Arduino.
  When the button is pressed, the buzzer should play the "Happy Birthday" melody once.
  After the melody finishes, the system waits until the button is pressed again to replay the tune.
*/

const int buzzerPin = 8;   // buzzer connected to pin 8
const int buttonPin = 2;   // button connected to pin 2

// Basic note frequencies (Hz)
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659

// Happy Birthday melody
int melody[] = {
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_C5, NOTE_B4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_D5, NOTE_C5,
  NOTE_G4, NOTE_G4, NOTE_G5, NOTE_E5, NOTE_C5, NOTE_B4, NOTE_A4,
  NOTE_F5, NOTE_F5, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_C5
};

//relative note lengths (4 = quarter, 8 = eighth, etc.)
int noteDurations[] = {
  4, 4, 2, 2, 2, 1,
  4, 4, 2, 2, 2, 1,
  4, 4, 2, 2, 2, 2, 1,
  4, 4, 2, 2, 2, 1
};

const int notesCount = sizeof(melody) / sizeof(melody[0]);

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // internal pull-up for button
}

void loop() {
  // Check if the button is pressed
  if (digitalRead(buttonPin) == LOW) {
    playHappyBirthday();

    // Wait for button release so it doesn't retrigger immediately
    while (digitalRead(buttonPin) == LOW) {
      delay(10);
    }
  }
}

void playHappyBirthday() {
  for (int i = 0; i < notesCount; i++) {
    int duration = 1000 / noteDurations[i];
    tone(buzzerPin, melody[i], duration);
    delay(duration * 1.3);  // small gap between notes
  }
  noTone(buzzerPin);
}

