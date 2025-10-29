#include "PinDefinitions.h"
#include <Arduino.h>

PinDefinitions::PinDefinitions():
    STATUS_LED_RED(PB5),
    STATUS_LED_GREEN(PB4),
    STATUS_LED_BLUE(PB3),

    IGNITER_0(PB10),
    IGNITER_1(PB11),

    IGNITER_SENSE_0(PB0),
    IGNITER_SENSE_1(PB1),

    SERVO(PA1),

    BUZZER(PA3),

    ESP32_CS(PA12),
    SD_CS(PA4),

    SCK(PA5),
    SDI(PA7),
    SDO(PA6),

    MCP_CS(PA11),
    MCP_INT(PA8),

    SDA(PB9),
    SCL(PB8),

    ARM(PB13)
{
    // Constructor
}

void PinDefinitions::setupPins() {
    pinMode(STATUS_LED_RED, OUTPUT);
    pinMode(STATUS_LED_GREEN, OUTPUT);
    pinMode(STATUS_LED_BLUE, OUTPUT);

    pinMode(IGNITER_0, OUTPUT);
    pinMode(IGNITER_1, OUTPUT);

    pinMode(IGNITER_SENSE_0, INPUT);
    pinMode(IGNITER_SENSE_1, INPUT);

    pinMode(SERVO, OUTPUT);

    pinMode(BUZZER, OUTPUT);

    pinMode(ESP32_CS, OUTPUT);
    pinMode(SD_CS, OUTPUT);

    pinMode(SCK, OUTPUT);
    pinMode(SDI, OUTPUT);
    pinMode(SDO, INPUT);

    pinMode(MCP_CS, OUTPUT);
    pinMode(MCP_INT, INPUT);

    pinMode(ARM, INPUT_PULLUP);

}

PinDefinitions PinDefs;