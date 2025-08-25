#include <Arduino.h>

int puls = 2; // Push button pin
int resetButton = 3; // LED reset pin
int LED = 10; // LED pin

unsigned long timeWorking = 0;   // Working seconds
unsigned long timeOffOn = 0;     // Stopped seconds
unsigned long prevSecond = 0;    // To count seconds
unsigned long startPress = 0;    // Start time of button press
bool isWorking = false;          // Actual state
bool ledOn = false;              // Permanent LED state

void setup() {
  Serial.begin(9600);
  pinMode(puls, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  int pulsValue = digitalRead(puls);
  int buttonValue = digitalRead(resetButton);
  unsigned long currentMillis = millis();

  if (pulsValue == LOW) { // Push button pressed
    if (!isWorking) {
      // Working state starts
      Serial.print("Temps entre parada-arranc: ");
      Serial.print(timeOffOn);
      Serial.println(" segons");
      timeOffOn = 0;
      prevSecond = currentMillis;
      startPress = currentMillis; // Start counting press time
    }

    // Count working seconds
    if (currentMillis - prevSecond >= 1000) {
      prevSecond = currentMillis;
      timeWorking++;
    }

    isWorking = true;

  } else { // Push button not pressed
    if (isWorking) {
      // Working state ends
      Serial.print("Temps treballat: ");
      Serial.print(timeWorking);
      Serial.println(" segons");

      // Turn on LED if worked less than 15 seconds
      if ((currentMillis - startPress) <= 15000) {
        digitalWrite(LED, HIGH);
        ledOn = true;
      }

      timeWorking = 0;
      prevSecond = currentMillis;
    }

    // Count stopped seconds
    if (currentMillis - prevSecond >= 1000) {
      prevSecond = currentMillis;
      timeOffOn++;
    }

    isWorking = false;
    startPress = 0; // Teset stopped time
  }

  if (buttonValue == LOW){
    // Reset button pressed, turn off LED
    digitalWrite(LED, LOW);
    ledOn = false;
  }
}