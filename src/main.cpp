#include "Adafruit_GFX.h"
#include "extras.h"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

// zdefiniowac jak jest podlaczony uklad
// SCL - 1
// SDA - 2
// LED - ??
// Przycisk - ??
#define LightPin D4
#define ButtonPin D3

// Screen setup
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    PinSetup(LightPin, ButtonPin);
    // display.clearDisplay();
    // display.display(); // zazwyczaj zmieniamy tylko zawartość buffora i wywołanie display() rysuje zawartość bufora na ekranie
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
    delay(1000);
    LightAndClockStart(LightPin);
    LightAndClockStop(LightPin, ButtonPin);
    delay(1000);
    InitializeTest(LightPin, ButtonPin, display, 5, (-1));
    delay(5000);
}