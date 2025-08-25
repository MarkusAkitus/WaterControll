#include <Arduino.h>

int puls = 2;         // Push button pin
int resetButton = 3;  // LED reset pin
int LED = 10;         // LED pin

unsigned long timeWorking = 0;   // Working seconds
unsigned long timeOffOn = 0;     // Stopped seconds
unsigned long prevSecond = 0;    // To count seconds
unsigned long startPress = 0;    // Start time of button press
unsigned long startMinute = 0;   // Start of 1-minute window
bool isWorking = false;          // Actual state
bool ledOn = false;              // Permanent LED state
int shortPressCount = 0;         // Number of short presses (≤15s)

void setup() {
  Serial.begin(9600);
  pinMode(puls, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  startMinute = millis(); // Start one-minute window
}

void loop() {
  int pulsValue = digitalRead(puls);
  int buttonValue = digitalRead(resetButton);
  unsigned long currentMillis = millis();

  if (pulsValue == LOW) { // Button pressed
    if (!isWorking) {
      Serial.print("Temps entre parada-arranc: ");
      Serial.print(timeOffOn);
      Serial.println(" segons");
      timeOffOn = 0;
      prevSecond = currentMillis;
      startPress = currentMillis;
    }

    // Count working seconds
    if (currentMillis - prevSecond >= 1000) {
      prevSecond = currentMillis;
      timeWorking++;
    }

    isWorking = true;

  } else { // Button released
    if (isWorking) {
      Serial.print("Temps treballat: ");
      Serial.print(timeWorking);
      Serial.println(" segons");

      // Count short presses (≤15s)
      if (timeWorking <= 15) {
        shortPressCount++;
      }

      timeWorking = 0;
      prevSecond = currentMillis;
      startPress = 0;
    }

    // Count stopped seconds
    if (currentMillis - prevSecond >= 1000) {
      prevSecond = currentMillis;
      timeOffOn++;
    }

    isWorking = false;
  }

  // Auto-reset one-minute window and short press count
  if (currentMillis - startMinute >= 60000) { // 60.000 ms = 1 minuto
    startMinute = currentMillis; // Reinicia ventana
    shortPressCount = 0;          // Reinicia contador
  }

  // Turn on LED if there are 2 short presses and LED is off
  if (shortPressCount >= 2 && !ledOn) {
    digitalWrite(LED, HIGH);
    ledOn = true;
  }

  // Resset LED
  if (buttonValue == LOW) {
    digitalWrite(LED, LOW);
    ledOn = false;
  }
}