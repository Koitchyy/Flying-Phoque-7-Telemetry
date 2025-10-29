#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H

class PinDefinitions {
public:
    // Constructor
    PinDefinitions();

    // Digital Pins
    const int STATUS_LED_RED;
    const int STATUS_LED_GREEN;
    const int STATUS_LED_BLUE;

    const int IGNITER_0;
    const int IGNITER_1;

    const int IGNITER_SENSE_0;
    const int IGNITER_SENSE_1;

    const int SERVO;

    const int BUZZER;

    const int ESP32_CS;
    const int SD_CS;

    const int SCK;
    const int SDI;
    const int SDO;

    const int MCP_CS;
    const int MCP_INT;

    const int SDA;
    const int SCL;

    const int ARM;

    void setupPins();
};

extern PinDefinitions PinDefs;

#endif