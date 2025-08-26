#include <Arduino.h>
#include "functions.h"

int puls = 2;         // Push button pin
int resetButton = 3;  // Reset button pin
int LED = 10;         // LED pin
int buzzer = 9;       // Buzzer pin
int Do = 261;         // Note frequencies

unsigned long timeWorking = 0;    // Working seconds
unsigned long timeOffOn = 0;      // Stopped seconds
unsigned long prevSecond = 0;     // To count seconds
unsigned long startPress = 0;     // Start time of button press
unsigned long startMinute = 0;    // Start of 1-minute window
bool isWorking = false;           // Actual state
bool ledOn = false;               // Permanent LED state
int shortPressCount = 0;          // Number of short presses (≤15s)
unsigned long lastBeep = 0;       // Last state change
bool buzzerOn = false;            // Buzzer state

void setup() {
  Serial.begin(9600);
  pinMode(puls, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(LED, LOW);
  digitalWrite(buzzer, HIGH);
  startMinute = millis(); // Start one-minute window
}

void loop() {
  int pulsValue = digitalRead(puls);
  // int buttonValue = digitalRead(resetButton);
  unsigned long currentMillis = millis();

  checkPuls(pulsValue, isWorking, startPress, currentMillis, timeWorking, prevSecond, timeOffOn, shortPressCount, ledOn, LED);

  // Count stopped seconds
  stoppedSeconds(timeOffOn, prevSecond);
  isWorking = false;

  // Auto-reset one-minute window and short press count
  autoreset(timeWorking, shortPressCount, startMinute);

  // Turn on LED & buzzer if there are 2 short presses and LED & buzzer are off
  checkSigns(timeWorking, shortPressCount, ledOn, LED, buzzer, Do, lastBeep, buzzerOn);

  // Reset LED & buzzer
  reset(timeWorking, shortPressCount, ledOn, LED, buzzer, Do, buzzerOn);
}