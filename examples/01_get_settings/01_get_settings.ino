#include <Arduino.h>
#include "pioneer_uart.h"

using namespace pioneer_uart;

PioneerWYT *wyt;

void setup()
{
    Serial.begin(115200);
    Serial1.begin(9600, SERIAL_8E1);

    wyt = new PioneerWYT(Serial1);
}

void loop()
{
    if (!wyt->pollState())
    {
        Serial.println("Failed to poll WYT state");
    }
    else
    {
        bool power = wyt->isPowerOn();
        OpMode mode = wyt->getMode();
        DegreesC set_temperature = wyt->getChosenTemperature();
        Serial.println("Status:");
        Serial.print(" - power ");
        Serial.println(power ? "on" : "off");
        Serial.print(" - mode ");
        switch (mode)
        {
        case OpMode::Auto:
            Serial.println("auto");
            break;
        case OpMode::Cool:
            Serial.println("cool");
            break;
        case OpMode::Heat:
            Serial.println("heat");
            break;
        case OpMode::Dehumidify:
            Serial.println("dehumidify");
            break;
        case OpMode::Fan:
            Serial.println("fan");
            break;
        }
        Serial.print(" - set temperature ");
        Serial.print(set_temperature, 1);
        Serial.println("ºC");
    }
    delay(4000);
}