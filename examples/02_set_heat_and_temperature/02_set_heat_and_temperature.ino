#include <Arduino.h>
#include "pioneer_uart.h"

using namespace pioneer_uart;

void setup()
{
    Serial.begin(115200);
    Serial1.begin(9600, SERIAL_8E1);

    PioneerWYT *wyt = new PioneerWYT(Serial1);
    if (!wyt->pollState())
    {
        Serial.println("Failed to poll WYT state");
    }
    else
    {
        wyt->setPowerOn(true);
        wyt->setMode(OpMode::Heat);
        wyt->setChosenTemperature(24.0);
        wyt->applySettings();
    }
    delay(4000);
}

void loop()
{
}