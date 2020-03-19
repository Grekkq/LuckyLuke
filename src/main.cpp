#include <ESP8266HTTPClient.h>
#include "SafeStorage.h"
#include "extras.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <FS.h>
#include <U8x8lib.h>
#include <Wire.h>
// ta biblioteka to same kłopty xd
#include <ESPAsyncWebServer.h>

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

AsyncWebServer server(80);
unsigned long myChannelNumber = SECRET_CH_ID;
const char *myWriteAPIKey = SECRET_WRITE_APIKEY;
// Screen setup
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);
// Font 5x7, 5x8, 8x13 can handle 16 chars per line

void setup()
{
    // Monitor Setup
    Serial.begin(115200);

    PinSetup(LightPin, ButtonPin);

    u8x8.begin();

    // File system
    if (!SetupSPIFFS())
        return;

    SetupWiFi(ssid, password);
    Serial.println(WiFi.localIP());
    ConfigureWebpages(server);
    server.begin();
    initializeTestFromWebFlag = false;
    u8x8.setFont(u8x8_font_8x13_1x2_f);
    u8x8.drawString(0, 0, " Aby rozpoczac  ");
    u8x8.drawString(0, 3, "wejdz na strone:");
    u8x8.drawString(0, 6, ("  " + WiFi.localIP().toString()).c_str());
}

void loop()
{
    if (initializeTestFromWebFlag)
    {
        InitializeTest(LightPin, ButtonPin, NumberOfMesurementsFromWeb, TimeBetweenLightingUpDiodeFromWeb, RandomTimeMinBoundFromWeb, RandomTimeMaxBoundFromWeb, myChannelNumber, myWriteAPIKey);
        initializeTestFromWebFlag = false;
    }
}