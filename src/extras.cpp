#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

// Stores time in miliseconds from button interrupt
volatile unsigned long FinishTime = 0;

volatile bool InterruptFlag = 0;

#define LightPin D4
// time between lighting up in miliseconds
#define RandomTimeLowerBound 3000
#define RandomTimeUpperBound 7000
// default button position
#define DefualtButtonPosition LOW
// mosfet activation
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
    attachInterrupt(digitalPinToInterrupt(ButtonPin), doOnButtonClick, RISING);
    InterruptFlag = false;
    return current;
}

int LightAndClockStop(int LightPin, int ButtonPin) {
    digitalWrite(LightPin, !ShortedMosfet);
    return millis();
}

int *InitializeTest(int LightPin, int ButtonPin, Adafruit_SSD1306 display, int NumberOfMeasurement, int TimeBetweenLightingUp) {
    int *Score = new int[NumberOfMeasurement];
    int RandomTime = 0;
    unsigned long StartTime = 0, wynik = 0;
    Serial.println("Test Initialization");
    for (int i = 0; i < NumberOfMeasurement; i++) {
        RandomTime = random(RandomTimeLowerBound, RandomTimeUpperBound);
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
        wynik = FinishTime - StartTime;
        Serial.println(wynik);
        Score[i] = (FinishTime - StartTime);
        Serial.println(Score[i]);
    }
    return Score;
}