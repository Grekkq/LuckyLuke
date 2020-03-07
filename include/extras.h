#include "Adafruit_GFX.h"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>
#include <ESPAsyncWebServer.h>

void ICACHE_RAM_ATTR handleInterrupt();
// Set LED pin as output and Button pin as input with pullup resistor
void PinSetup(int LightPin, int ButtonPin);
// Turn on LED and return time since boot in miliseconds
unsigned long LightAndClockStart(int LightPin, int ButtonPin);
// Turn off LED and return time since boot in miliseconds
int LightAndClockStop(int LightPin, int ButtonPin);
// Return vecotr of recorded times, Time in miliseconds -1 for random
int* InitializeTest(int LightPin, int ButtonPin, Adafruit_SSD1306 display, int NumberOfMeasurement, int TimeBetweenLightingUp);