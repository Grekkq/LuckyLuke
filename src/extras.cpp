#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

// time between lighting up in miliseconds
#define RandomTimeLowerBound 300
#define RandomTimeUpperBound 1300
// default button position
#define DefualtButtonPosition LOW
// mosfet activation
#define ShortedMosfet LOW

void PinSetup(int LightPin, int ButtonPin) {

    pinMode(LightPin, OUTPUT);
    pinMode(ButtonPin, INPUT_PULLUP);
}

int LightAndClockStart(int LightPin) {

    digitalWrite(LightPin, ShortedMosfet);
    return millis();
}

int LightAndClockStop(int LightPin, int ButtonPin) {

    while (digitalRead(ButtonPin) == DefualtButtonPosition) {
        ESP.wdtFeed();
    }
    digitalWrite(LightPin, !ShortedMosfet);
    return millis();
}

int* InitializeTest(int LightPin, int ButtonPin, Adafruit_SSD1306 display, int NumberOfMeasurement, int TimeBetweenLightingUp) {
    // display.write('test\ntest');
    int * Score = new int[NumberOfMeasurement];
    int RandomTime, StartTime, FinishTime = 0;
    for (int i = 0; i < NumberOfMeasurement; i++) {
        RandomTime = random(RandomTimeLowerBound, RandomTimeUpperBound);
        if (TimeBetweenLightingUp == (-1))
            delay(RandomTime);
        else
            delay(TimeBetweenLightingUp);

        StartTime = LightAndClockStart(LightPin);
        FinishTime = LightAndClockStop(LightPin, ButtonPin);
        Score[i] = (FinishTime - StartTime);
    }
    return Score;
}