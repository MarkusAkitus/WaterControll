// Put function code here
#include "functions.h"

void startWorking(int &timeOffOn, unsigned long &prevSecond, unsigned long &startPress, bool &isWorking) {
    if (!isWorking) {
        // Working state started
        Serial.print("Temps entre parada-arranc: ");
        Serial.print(timeOffOn);
        Serial.println(" segons");
        timeOffOn = 0;
        prevSecond = millis();
        startPress = millis();
    }
  
}

void workingSeconds(unsigned long &timeWorking, unsigned long &prevSecond) {
  // Count working seconds
  if (millis() - prevSecond >= 1000) {
    prevSecond = millis();
    timeWorking++;
  }
}

void stopWorking(int &timeWorking, int &shortPressCount) {
  // Count short presses (≤15s)
  if (timeWorking <= 15) {
    shortPressCount++;
  }
  timeWorking = 0;
}

void shortPresses(int &shortPressCount, int &timeWorking) {
  // Count short presses (≤15s)
  if (timeWorking <= 15) {
    shortPressCount++;
  }
}

void stoppedSeconds(unsigned long &timeOffOn, unsigned long &prevSecond) {
  // Count stopped seconds
  if (millis() - prevSecond >= 1000) {
    prevSecond = millis();
    timeOffOn++;
  }
}

void autoreset(int &timeWorking, int &shortPressCount, unsigned long &startMinute) {
  // Auto-reset after one minute
  if (millis() - startMinute >= 60000) {
    timeWorking = 0;
    shortPressCount = 0;
    startMinute = millis(); // Restart one-minute window
  }
}

void checkLED(int &timeWorking, int &shortPressCount, bool &ledOn, int LED) {
  // LED control based on working time and short presses
  if (timeWorking > 0) {
    if (!ledOn) {
      digitalWrite(LED, HIGH); // Turn on LED
      ledOn = true;
    }
  } else {
    if (ledOn) {
      digitalWrite(LED, LOW); // Turn off LED
      ledOn = false;
    }
  }

  if (shortPressCount >= 3) {
    if (!ledOn) {
      digitalWrite(LED, HIGH); // Turn on LED
      ledOn = true;
    }
  }
}

void resetLED(int &timeWorking, int &shortPressCount, bool &ledOn, int LED) {
  // Reset LED and short press count
  if (ledOn) {
    digitalWrite(LED, LOW); // Turn off LED
    ledOn = false;
  }
  shortPressCount = 0; // Reset short press count
}

void checkPuls(int pulsValue, bool &isWorking, unsigned long &startPress, unsigned long currentMillis, unsigned long &timeWorking, unsigned long &prevSecond, unsigned long &timeOffOn, int &shortPressCount, bool &ledOn, int LED) {
  if (pulsValue == LOW) { // Button pressed
    // Working state started
    startWorking(timeOffOn, prevSecond, startPress, isWorking);

    // Count working seconds
    workingSeconds(timeWorking, prevSecond);

    isWorking = true;

  } else { // Button released
    if (isWorking) {
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