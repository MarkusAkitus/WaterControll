#include <Arduino.h>
#include "functions.h"

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

  checkPuls(pulsValue, isWorking, startPress, currentMillis, timeWorking, prevSecond, timeOffOn, shortPressCount, ledOn, LED);

  // Count stopped seconds
  stoppedSeconds(timeOffOn, prevSecond);
  isWorking = false;

  // Auto-reset one-minute window and short press count
  autoreset(timeWorking, shortPressCount, startMinute);

  // Turn on LED if there are 2 short presses and LED is off
  checkLED(timeWorking, shortPressCount, ledOn, LED);

  // Reset LED
  resetLED(timeWorking, shortPressCount, ledOn, LED);
}