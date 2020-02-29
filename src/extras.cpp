#include "LinkedList.h"
#include <Arduino.h>
// time between lighting up in miliseconds
#define RandomTimeLowerBound 300
#define RandomTimeUpperBound 1300
// default button position
#define DefualtButtonPosition HIGH

void PinSetup(int LightPin, int ButtonPin) {

    pinMode(LightPin, OUTPUT);
    pinMode(ButtonPin, INPUT_PULLUP);
}

int LightAndClockStart(int LightPin) {

    digitalWrite(LightPin, HIGH);
    return millis();
}

int LightAndClockStop(int LightPin, int ButtonPin) {

    while (digitalRead(ButtonPin) == DefualtButtonPosition) {
    }
    digitalWrite(LightPin, LOW);
    return millis();
}

LinkedList<int> InitializeTest(int LightPin, int ButtonPin, int NumberOfMeasurement, int TimeBetweenLightingUp) {

    LinkedList<int> Score;
    int RandomTime, StartTime, FinishTime = 0;
    for (int i = 0; i < NumberOfMeasurement; i++) {
        RandomTime = random(RandomTimeLowerBound, RandomTimeUpperBound);
        if (TimeBetweenLightingUp == (-1))
            delay(RandomTime);
        else
            delay(TimeBetweenLightingUp);

        StartTime = LightAndClockStart(LightPin);
        FinishTime = LightAndClockStop(LightPin, ButtonPin);
        Score.Append(FinishTime - StartTime);
    }
    return Score;
}