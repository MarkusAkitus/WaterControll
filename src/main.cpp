#include <Arduino.h>

int puls = 2; // Digital pin for the pulse sensor
int LED = 10; // LED pin

unsigned long timeWorking = 0;  // Time spent working in ms
unsigned long timeOffOn = 0;    // Time spent non-working in seconds
unsigned long prevSecond = 0;   // Seconds counter
bool isWorking = false;         // Actual working state

void setup() {
  Serial.begin(9600);
  pinMode(puls, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
}

void loop() {
  int pulsValue = digitalRead(puls);
  unsigned long currentMillis = millis();

  if (pulsValue == LOW && currentMillis <= 15000) {
    // Working state
    digitalWrite(LED, HIGH);

    if (!isWorking) {
      // If before it wasn't working but now it is → show timeOffOn
      Serial.print("Temps entre parada-arranc: ");
      Serial.print(timeOffOn);
      Serial.println(" segons");

      // Reset non-working time counter
      timeOffOn = 0;
      prevSecond = currentMillis;
    }
    isWorking = true;

  } else {
    // Non-working state
    digitalWrite(LED, LOW);

    if (isWorking) {
      // Out of working state
      prevSecond = currentMillis;
    }

    // Count non-working time
    if (currentMillis - prevSecond >= 1000) {
      prevSecond = currentMillis;
      timeOffOn++;
    }

    isWorking = false;
  }
}