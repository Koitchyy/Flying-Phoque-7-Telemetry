#pragma once
#include <Arduino.h>
#include <Wire.h>

class TeensyBridge {
public:
    TeensyBridge(uint8_t address = 0x42) : address_(address) {}

    bool begin() {
        delay(300);
        Serial1.println("Initializing Teensy bridge...");
        Wire.beginTransmission(address_);
        uint8_t result = Wire.endTransmission();
        if (result == 0) {
            Serial1.print("Teensy found at I2C address 0x");
            Serial1.println(address_, HEX);
            return true;
        } else {
            Serial1.println("Teensy not responding on I2C!");
            return false;
        }
    }

    bool sendTelemetry(const uint8_t* data, size_t len) {
        Wire.beginTransmission(address_);
        Wire.write(data, len);
        return (Wire.endTransmission() == 0);
    }

private:
    uint8_t address_;
};
