#include "Adafruit_GFX.h"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>
#include <ESPAsyncWebServer.h>

extern volatile int NumberOfMesurementsFromWeb;
extern volatile int TimeBetweenLightingUpDiodeFromWeb;
extern volatile int RandomTimeMinBoundFromWeb;
extern volatile int RandomTimeMaxBoundFromWeb;
extern volatile bool initializeTestFromWebFlag;

void ICACHE_RAM_ATTR doOnButtonClick();
// Set LED pin as output and Button pin as input with pullup resistor
void PinSetup(int LightPin, int ButtonPin);
// Starts SPIFFS and return false if error occurs
bool SetupSPIFFS();
// Sets up WiFi connection
void SetupWiFi(const char * ssid, const char * password);
// Interactions between User and Server
void ConfigureWebpages(AsyncWebServer & server);
// Turn on LED, enable interrupt on button click and return time at the call of this function
unsigned long LightAndClockStart(int LightPin, int ButtonPin);
// Return vecotr of recorded times, Time in miliseconds -1 for random
int* InitializeTest(int LightPin, int ButtonPin, Adafruit_SSD1306 display, int NumberOfMeasurement, int TimeBetweenLightingUp, int RandomTimeMinBound, int RandomTimeMaxBound);