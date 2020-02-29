#include "LinkedList.h"
#include <Arduino.h>

// Set LED pin as output and Button pin as input with pullup resistor
void PinSetup(int LightPin, int ButtonPin);
// Turn on LED and return time since boot in miliseconds
int LightAndClockStart(int LightPin);
// Turn off LED and return time since boot in miliseconds
int LightAndClockStop(int LightPin, int ButtonPin);
// Return vecotr of recorded times, Time in miliseconds -1 for random
LinkedList<int> InitializeTest(int LightPin, int ButtonPin, int NumberOfMeasurement, int TimeBetweenLightingUp)