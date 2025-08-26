// Define functions here
#include "functions.h"

void startWorking(unsigned long &timeOffOn, unsigned long &prevSecond, unsigned long &startPress, bool &isWorking)
{
  if (!isWorking)
  {
    // Working state started
    Serial.print("Temps entre parada-arranc: ");
    Serial.print(timeOffOn);
    Serial.println(" segons");
    timeOffOn = 0;
    prevSecond = millis();
    startPress = millis();
  }
}

void workingSeconds(unsigned long &timeWorking, unsigned long &prevSecond)
{
  // Count working seconds
  if (millis() - prevSecond >= 1000)
  {
    prevSecond = millis();
    timeWorking++;
  }
}

void stopWorking(unsigned long &timeWorking, int &shortPressCount)
{
  // Count short presses (≤15s)
  if (timeWorking <= 15)
  {
    shortPressCount++;
  }
  timeWorking = 0;
}

void shortPresses(int &shortPressCount, unsigned long &timeWorking)
{
  // Count short presses (≤15s)
  if (timeWorking <= 15)
  {
    shortPressCount++;
  }
}

void stoppedSeconds(unsigned long &timeOffOn, unsigned long &prevSecond)
{
  // Count stopped seconds
  if (millis() - prevSecond >= 1000)
  {
    prevSecond = millis();
    timeOffOn++;
  }
}

void autoreset(unsigned long &timeWorking, int &shortPressCount, unsigned long &startMinute)
{
  // Auto-reset after one minute
  if (millis() - startMinute >= 60000)
  {
    timeWorking = 0;
    shortPressCount = 0;
    startMinute = millis(); // Restart one-minute window
  }
}

void checkSigns(unsigned long &timeWorking, int &shortPressCount, bool &ledOn, int LED, int buzzer, int Do, unsigned long &lastBeep, bool &buzzerOn)
{
  if ((timeWorking > 0 || shortPressCount >= 3) && !ledOn)
  {
    digitalWrite(LED, HIGH);
    ledOn = true; // LED on
  }

  // If LED = on → buzzer beeps every second
  if (ledOn)
  {
    unsigned long currentMillis = millis();

    if (buzzerOn && currentMillis - lastBeep >= 500)
    {
      // If it is toning during 500ms → turn off
      noTone(buzzer);
      buzzerOn = false;
      lastBeep = currentMillis;
    }
    else if (!buzzerOn && currentMillis - lastBeep >= 500)
    {
      // If it is stopped durint 500 ms → turn on
      tone(buzzer, Do);
      buzzerOn = true;
      lastBeep = currentMillis;
    }
  }
}

void reset(unsigned long &timeWorking, int &shortPressCount, bool &ledOn, int LED, int buzzer, int Do, bool &buzzerOn)
{
  if (ledOn)
  {
    digitalWrite(LED, LOW);
    noTone(buzzer);
    ledOn = false;
    buzzerOn = false;
  }
  shortPressCount = 0;
}

void checkPuls(int pulsValue, bool &isWorking, unsigned long &startPress, unsigned long currentMillis, unsigned long &timeWorking, unsigned long &prevSecond, unsigned long &timeOffOn, int &shortPressCount, bool &ledOn, int LED)
{
  if (pulsValue == LOW)
  { // Button pressed
    // Working state started
    startWorking(timeOffOn, prevSecond, startPress, isWorking);

    // Count working seconds
    workingSeconds(timeWorking, prevSecond);

    isWorking = true;
  }
  else
  { // Button released
    if (isWorking)
    {
      // Working state ended
      stopWorking(timeWorking, shortPressCount);

      // Count short presses (≤15s)
      shortPresses(shortPressCount, timeWorking);

      timeWorking = 0;
      prevSecond = millis();
      startPress = 0;
    }

    // Count stopped seconds
    stoppedSeconds(timeOffOn, prevSecond);
    isWorking = false;
  }
}