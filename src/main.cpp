#include "Adafruit_GFX.h"
#include "extras.h"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <FS.h>
// ta biblioteka to same kłopty xd
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include "SafeStorage.h"

// Pin Assigments
#define LightPin D4
#define ButtonPin D3

// WiFi config in SafeStorage.h Template:
// #ifndef SAFESTORAGE_H
// #define SAFESTORAGE_H
// // WiFi config
// const char *ssid = "Koparka Apokalipsy";
// const char *password = "xd";
// #endif
// Server Setup
AsyncWebServer server(80);


// Screen setup
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// do usuniecia jak ogarniemy przekazywanie do strony
// String ledState;
// Replaces placeholder with LED state value
// String processor(const String &var) {
//     Serial.println(var);
//     if (var == "STATE") {
//         if (digitalRead(LightPin)) {
//             ledState = "ON";
//         } else {
//             ledState = "OFF";
//         }
//         Serial.println(ledState);
//         return ledState;
//     }
// }

void setup() {
    // Monitor Setup
    Serial.begin(115200);

    PinSetup(LightPin, ButtonPin);

    // display.clearDisplay();
    // display.display(); // zazwyczaj zmieniamy tylko zawartość buffora i wywołanie display() rysuje zawartość bufora na ekranie

    // File system
    if(!SetupSPIFFS())
        return;

    SetupWiFi(ssid, password);
    Serial.println(WiFi.localIP());
    ConfigureWebpages(server);
    server.begin();
    initializeTestFromWebFlag = false;
}

void loop() {
    if (initializeTestFromWebFlag) {
        InitializeTest(LightPin, ButtonPin, display, NumberOfMesurementsFromWeb, TimeBetweenLightingUpDiodeFromWeb, RandomTimeMinBoundFromWeb, RandomTimeMaxBoundFromWeb);
        initializeTestFromWebFlag = false;
    }
}