#include "extras.h"
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <U8x8lib.h>

volatile int NumberOfMesurementsFromWeb = 0;
volatile int TimeBetweenLightingUpDiodeFromWeb = 0;
volatile int RandomTimeMinBoundFromWeb = 0;
volatile int RandomTimeMaxBoundFromWeb = 0;
volatile bool initializeTestFromWebFlag = false;
volatile int *Score;
// Stores time in miliseconds from button interrupt
volatile unsigned long FinishTime = 0;
volatile bool InterruptFlag = 0;
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8p(U8X8_PIN_NONE);

#define EdgeOnButtonPress RISING
#define ShortedMosfet HIGH

void ICACHE_RAM_ATTR doOnButtonClick() {
    FinishTime = millis();
    InterruptFlag = true;
}

void PinSetup(int LightPin, int ButtonPin) {
    pinMode(LightPin, OUTPUT);
    pinMode(ButtonPin, INPUT_PULLUP);
    u8x8p.begin();
    u8x8p.setFont(u8x8_font_8x13_1x2_f);
}

bool SetupSPIFFS() {
    Serial.println("Startin SPIFFS...");
    if (!SPIFFS.begin()) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return false;
    }
    return true;
}

void SetupWiFi(const char *ssid, const char *password) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
}

unsigned long LightAndClockStart(int LightPin, int ButtonPin) {
    unsigned long current = millis();
    digitalWrite(LightPin, ShortedMosfet);
    attachInterrupt(digitalPinToInterrupt(ButtonPin), doOnButtonClick, EdgeOnButtonPress);
    InterruptFlag = false;
    return current;
}

void ConfigureWebpages(AsyncWebServer &server) {
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
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
        request->send(SPIFFS, "/measurement.html");
    });

    server.on("/times", HTTP_GET, [](AsyncWebServerRequest *request) {
        String response = String(NumberOfMesurementsFromWeb);
        for (int i = 0; i < NumberOfMesurementsFromWeb; i++) {
            response += "," + String(Score[i]);
        }
        request->send_P(200, "text/plain", response.c_str());
    });

    server.on("/home", HTTP_GET, [](AsyncWebServerRequest *request) {
        u8x8p.clearDisplay();
        u8x8p.drawString(0, 0, "Ustaw parametry ");
        u8x8p.drawString(0, 3, " i rozpocznij   ");
        u8x8p.drawString(0, 6, "kolejne badanie ");
        request->send(SPIFFS, "/index.html");
    });

    server.on("/about", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/about.html");
    });
}

void ResultsOnOLED(volatile int *tab, int size, unsigned long myChannelNumber, const char *myWriteAPIKey) {
    int min = tab[0];
    int max = tab[0];
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += tab[i];
        if (tab[i] < min) {
            min = tab[i];
        } else if (tab[i] > max) {
            max = tab[i];
        }
    }
    float avg = sum / size;

    u8x8p.clearDisplay();
    u8x8p.drawString(0, 2, "   Zakonczono   ");
    u8x8p.drawString(0, 4, "    badanie     ");
    delay(4000);
    u8x8p.clearDisplay();
    u8x8p.drawString(0, 0, "     Wyniki     ");
    u8x8p.drawString(0, 2, String("AVG: " + String(avg) + " ms").c_str());
    u8x8p.drawString(0, 4, String("MAX: " + String(max) + " ms").c_str());
    u8x8p.drawString(0, 6, String("MIN: " + String(min) + " ms").c_str());
<<<<<<< HEAD
=======
    delay(1000);
    // ThingSpeak.setField(1, avg);
    delay(1000);
    // ThingSpeak.setField(2, min);
    // delay(1000);
    // ThingSpeak.setField(3, max);
    // delay(1000);
    // Serial.println("Wysylanie: ");
    // int test = ThingSpeak.writeField(1022117, 1, 5, "T81H9K2H99ML7SW5");
    // Serial.println(test);
    delay(1000);
>>>>>>> 398f66e5ba7f42c77e56e37da463066a9292144d
}

void InitializeTest(int LightPin, int ButtonPin, int NumberOfMeasurement, int TimeBetweenLightingUp, int RandomTimeMinBound, int RandomTimeMaxBound, unsigned long myChannelNumber, const char *myWriteAPIKey) {
    // Serial.println("Wysylanie: ");
    ThingSpeak.writeField(1022117, 1, float(5), "Y2LG7XRCFUB669W2");
    // Serial.println(test);
    u8x8p.clearDisplay();
    u8x8p.drawString(0, 2, "   Rozpoczynam  ");
    u8x8p.drawString(0, 4, "    badanie     ");
    delay(2000);

    u8x8p.clearDisplay();
    u8x8p.drawString(0, 3, "  Przygotuj sie ");
    delay(2000);

    u8x8p.clearDisplay();
    u8x8p.setFont(u8x8_font_inb46_4x8_r);
    u8x8p.drawString(0, 0, "4");
    delay(1000);

    u8x8p.clearDisplay();
    u8x8p.drawString(0, 0, " 3");
    delay(1000);

    u8x8p.clearDisplay();
    u8x8p.drawString(0, 0, "  2");
    delay(1000);

    u8x8p.clearDisplay();
    u8x8p.drawString(0, 0, "   1");
    delay(1000);

    u8x8p.setFont(u8x8_font_8x13_1x2_f);
    u8x8p.clearDisplay();
    u8x8p.drawString(0, 2, "  Trwa pomiar:  ");

    Score = new int[NumberOfMeasurement];
    int RandomTime = 0;
    unsigned long StartTime = 0, ElapsedTime = 0;
    Serial.println("Test Initialization");
    for (int i = 0; i < NumberOfMeasurement; i++) {
        u8x8p.clearLine(4);
        u8x8p.drawString(0, 4, String("       " + String(i + 1)).c_str());
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
    ResultsOnOLED(Score, NumberOfMeasurement, myChannelNumber, myWriteAPIKey);
}