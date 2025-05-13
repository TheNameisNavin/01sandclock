#include <TM1637Display.h>

#define CLK 2   // TM1637 CLK pin
#define DIO 3   // TM1637 DIO pin
#define BUTTON_PIN 4 // Flip button
#define BUZZER_PIN 5 // Optional buzzer

TM1637Display display(CLK, DIO);

const int startMinutes = 1; // 1-minute sand clock
int remainingSeconds = startMinutes * 60;
bool running = true;
unsigned long lastUpdate = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  display.setBrightness(0x0f);
  display.showNumberDecEx(remainingSeconds, 0b01000000, true);
}

void loop() {
  // Button pressed: reset timer
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(200); // debounce
    resetTimer();
  }

  if (running && millis() - lastUpdate >= 1000) {
    lastUpdate = millis();
    remainingSeconds--;

    if (remainingSeconds <= 0) {
      beep();
      resetTimer();
    }

    displayTime(remainingSeconds);
  }
}

void resetTimer() {
  remainingSeconds = startMinutes * 60;
  displayTime(remainingSeconds);
  running = true;
}

void displayTime(int seconds) {
  int mins = seconds / 60;
  int secs = seconds % 60;
  int timeVal = mins * 100 + secs;
  display.showNumberDecEx(timeVal, 0b01000000, true); // colon between MM:SS
}

void beep() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }
}
