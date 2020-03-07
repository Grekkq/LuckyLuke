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

// button interrupt
// volatile int FinishTime = 0;

int ilosc = 0;
int czas = 0;

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

// Screen etup
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String ledState;

// Replaces placeholder with LED state value
String processor(const String &var) {
    Serial.println(var);
    if (var == "STATE") {
        if (digitalRead(LightPin)) {
            ledState = "ON";
        } else {
            ledState = "OFF";
        }
        Serial.println(ledState);
        return ledState;
    }
}




void setup() {
    // print setup
    Serial.begin(115200);
    PinSetup(LightPin, ButtonPin);

    // display.clearDisplay();
    // display.display(); // zazwyczaj zmieniamy tylko zawartość buffora i wywołanie display() rysuje zawartość bufora na ekranie
    
    //Serwer WWW
    // if (!SPIFFS.begin()) {
    //     Serial.println("An Error has occurred while mounting SPIFFS");
    //     return;
    // }

    // WiFi.begin(ssid, password);
    // while (WiFi.status() != WL_CONNECTED) {
    //     delay(1000);
    //     Serial.println("Connecting to WiFi..");
    // }

    // Serial.println(WiFi.localIP());

    // // Route for root / web page
    // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     request->send(SPIFFS, "/index.html", String(), false, processor);
    // });

    // // Route to load style.css file
    // server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     request->send(SPIFFS, "/style.css", "text/css");
    // });
    // // Route to set GPIO to HIGH
    // server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     digitalWrite(LightPin, HIGH);
    //     request->send(SPIFFS, "/index.html", String(), false, processor);
    // });

    // // Route to set GPIO to LOW
    // server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     digitalWrite(LightPin, LOW);
    //     request->send(SPIFFS, "/index.html", String(), false, processor);
    // });

    // server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     int paramsNr = request->params();
    //     Serial.println(paramsNr);

    //     // for(int i=0;i<paramsNr;i++){
    //     //   AsyncWebParameter* p = request->getParam(i);
    //     //   Serial.print("Param name: ");
    //     //   Serial.println(p->name());
    //     //   Serial.print("Param value: ");
    //     //   Serial.println(p->value());
    //     //   Serial.println("------");
    //     // }

    //     ilosc = (request->getParam(0)->value()).toInt();
    //     czas = (request->getParam(1)->value()).toInt();
    //     Serial.println(ilosc);
    //     Serial.println(czas);

    //     request->send(SPIFFS, "/measurement.html", String(), false, processor);
    //     int *wyniki = InitializeTest(LightPin, ButtonPin, display, ilosc, czas);
    //     for (int i = 0; i < ilosc; i++) {
    //         Serial.println(wyniki[i]);
    //     }
    // });

    // server.begin();
}

void loop() {
    // if(digitalRead(ButtonPin)==LOW) {
    //     digitalWrite(LightPin, HIGH);
    // } else
    // {
    //     digitalWrite(LightPin, LOW);
    // }

    // digitalWrite(LightPin, HIGH);
    // delay(500);
    // digitalWrite(LightPin, LOW);
    // delay(500);

    // Basic Test xD
    // delay(1000);
    // LightAndClockStart(LightPin);
    // LightAndClockStop(LightPin, ButtonPin);
    delay(1000);
    InitializeTest(LightPin, ButtonPin, display, 5, (-1));
    delay(5000);
}