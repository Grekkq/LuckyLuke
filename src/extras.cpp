#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "extras.h"

volatile int NumberOfMesurementsFromWeb = 0;
volatile int TimeBetweenLightingUpDiodeFromWeb = 0;
volatile int RandomTimeMinBoundFromWeb = 0;
volatile int RandomTimeMaxBoundFromWeb = 0;
volatile bool initializeTestFromWebFlag = false;
volatile int *Score;
// Stores time in miliseconds from button interrupt
volatile unsigned long FinishTime = 0;
volatile bool InterruptFlag = 0;


#define EdgeOnButtonPress RISING
#define ShortedMosfet HIGH

void ICACHE_RAM_ATTR doOnButtonClick() {
    FinishTime = millis();
    InterruptFlag = true;
}

void PinSetup(int LightPin, int ButtonPin) {
    pinMode(LightPin, OUTPUT);
    pinMode(ButtonPin, INPUT_PULLUP);
}

bool SetupSPIFFS() {
    Serial.println("Startin SPIFFS...");
    if (!SPIFFS.begin()) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return false;
    }
    return true;
}

void SetupWiFi(const char * ssid, const char * password) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
}

unsigned long LightAndClockStart(int LightPin,  int ButtonPin) {
    unsigned long current = millis();
    digitalWrite(LightPin, ShortedMosfet);
    attachInterrupt(digitalPinToInterrupt(ButtonPin), doOnButtonClick, EdgeOnButtonPress);
    InterruptFlag = false;
    return current;
}

void ConfigureWebpages(AsyncWebServer & server) {
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        // request->send(SPIFFS, "/index.html", String(), false, processor);
        request->send(SPIFFS, "/index.html");
    });
    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/style.css", "text/css");
    });
    // Route to initialize test and redirecting to waiting page
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
        NumberOfMesurementsFromWeb = (request->getParam(0)->value()).toInt();
        TimeBetweenLightingUpDiodeFromWeb = (request->getParam(1)->value()).toInt();
        RandomTimeMinBoundFromWeb = (request->getParam(2)->value()).toInt();
        RandomTimeMaxBoundFromWeb = (request->getParam(3)->value()).toInt();
        Serial.print("NumberOfMesurementsFromWeb: ");
        Serial.println(NumberOfMesurementsFromWeb);
        Serial.print("TimeBetweenLightingUpDiodeFromWeb: ");
        Serial.println(TimeBetweenLightingUpDiodeFromWeb);
        Serial.print("RandomTimeMinBoundFromWeb: ");
        Serial.println(RandomTimeMinBoundFromWeb);
        Serial.print("RandomTimeMaxBoundFromWeb: ");
        Serial.println(RandomTimeMaxBoundFromWeb);
        initializeTestFromWebFlag = true;
        // request->send(SPIFFS, "/measurement.html", String(), false, processor);
        request->send(SPIFFS, "/measurement.html");
    });

    server.on("/times", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.print("Test przycisku wez i mi daj te wyniki: ");
        String response = String(NumberOfMesurementsFromWeb);
        for(int i=0; i< NumberOfMesurementsFromWeb; i++) {
            response += "," + String(Score[i]);
        }
        // int str_len = response.length() + 1;
        // char char_array[str_len];
        // request->send_P(200, "text/plain", response.toCharArray(char_array, str_len) );
        request->send_P(200, "text/plain", response.c_str() );

    });
}

void InitializeTest(int LightPin, int ButtonPin, Adafruit_SSD1306 display, int NumberOfMeasurement, int TimeBetweenLightingUp, int RandomTimeMinBound, int RandomTimeMaxBound) {
    Score = new int[NumberOfMeasurement];
    int RandomTime = 0;
    unsigned long StartTime = 0, ElapsedTime = 0;
    Serial.println("Test Initialization");
    for (int i = 0; i < NumberOfMeasurement; i++) {
        RandomTime = random(RandomTimeMinBound, RandomTimeMaxBound);
        if (TimeBetweenLightingUp == (-1))
            delay(RandomTime);
        else
            delay(TimeBetweenLightingUp);

        Serial.println("Dioda ON");
        StartTime = LightAndClockStart(LightPin, ButtonPin);
        while (!InterruptFlag) {
            delay(0);
        }
        digitalWrite(LightPin, !ShortedMosfet);
        ElapsedTime = FinishTime - StartTime;
        Serial.print("Elapsed Time: ");
        Serial.println(ElapsedTime);
        Score[i] = (ElapsedTime);
    }
}