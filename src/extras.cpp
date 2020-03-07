#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
volatile int FinishTime = 0;
volatile bool InterruptFlag = 0;
#define LightPin D4
// time between lighting up in miliseconds
#define RandomTimeLowerBound 3000
#define RandomTimeUpperBound 7000
// default button position
#define DefualtButtonPosition LOW
// mosfet activation
#define ShortedMosfet HIGH

void ICACHE_RAM_ATTR handleInterrupt() {
    FinishTime = millis();
    digitalWrite(LightPin, !ShortedMosfet);
    InterruptFlag = true;
}

void PinSetup(int LightPin, int ButtonPin) {

    pinMode(LightPin, OUTPUT);
    pinMode(ButtonPin, INPUT_PULLUP);
}

int LightAndClockStart(int LightPin,  int ButtonPin) {
    digitalWrite(LightPin, ShortedMosfet);
    attachInterrupt(digitalPinToInterrupt(ButtonPin), handleInterrupt, RISING);
    InterruptFlag = false;
    return millis();
}

int LightAndClockStop(int LightPin, int ButtonPin) {
    
    // while (digitalRead(ButtonPin) == DefualtButtonPosition) {
    //     ESP.wdtFeed();
    // }
    // digitalWrite(LightPin, !ShortedMosfet);
    return millis();
}

int *InitializeTest(int LightPin, int ButtonPin, Adafruit_SSD1306 display, int NumberOfMeasurement, int TimeBetweenLightingUp) {
    // display.write('test\ntest');
    int *Score = new int[NumberOfMeasurement];
    int RandomTime, StartTime = 0;
    Serial.println("Test start");
    for (int i = 0; i < NumberOfMeasurement; i++) {
        RandomTime = random(RandomTimeLowerBound, RandomTimeUpperBound);
        if (TimeBetweenLightingUp == (-1))
            delay(RandomTime);
        else
            delay(TimeBetweenLightingUp);
        
        Serial.println("Dioda ON");
        StartTime = LightAndClockStart(LightPin, ButtonPin);
        while (!InterruptFlag) {
        delay(1);
        }
        Score[i] = (FinishTime - StartTime);
        Serial.println(Score[i]);
    }
    return Score;
}