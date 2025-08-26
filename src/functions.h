// Declare functions here

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>

void startWorking(unsigned long &timeOffOn, unsigned long &prevSecond, unsigned long &startPress, bool &isWorking);
void workingSeconds(unsigned long &timeWorking, unsigned long &prevSecond);
void stopWorking(int timeWorking, int &shortPressCount);
void shortPresses(int &shortPressCount, int timeWorking);
void stoppedSeconds(int timeOffOn, unsigned long &prevSecond);
void autoreset(int timeWorking, int &shortPressCount, unsigned long &startMinute);
void checkSigns(int timeWorking, int &shortPressCount, bool &ledOn, int LED, int buzzer, int Do, unsigned long &lastBeep, bool &buzzerOn);
void reset(int timeWorking, int &shortPressCount, bool &ledOn, int LED, int buzzer, int Do, bool &buzzerOn);
void checkPuls(int pulsValue, bool &isWorking, unsigned long &startPress, unsigned long currentMillis, unsigned long &timeWorking, unsigned long &prevSecond, unsigned long &timeOffOn, int &shortPressCount, bool &ledOn, int LEDs);

#endif