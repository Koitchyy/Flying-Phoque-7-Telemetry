#include "Lps22.h"
// #include "ShitlSerial.h"

Lps22::Lps22(uint8_t address)
{
    LPS22_ADDRESS = address;
}

bool Lps22::begin()
{

    uint8_t deviceId = readRegister(0x0F);
    if (deviceId != 0xB3)
    {
        Serial1.print("Error: Could not find Lps22 sensor with address 0x");
        Serial1.print(LPS22_ADDRESS, HEX);
        Serial1.print(", received 0x");
        Serial1.println(deviceId, HEX);
        return false;
    }

    writeRegister(0x10, 0x90); // Power on the sensor
    return true;
}

void Lps22::readPressure(float *pressure)
{
    uint8_t buffer[3];

    // Start I2C transmission and write the register address
    Wire.beginTransmission(LPS22_ADDRESS);
    Wire.write(0x28); // Start reading from data registers
    if (Wire.endTransmission(false) != 0)
    { // Check if transmission failed
        Serial1.println("Error: Failed to write to Lps22!");
        return; // Exit if transmission failed
    }

    // Request 3 bytes of data from the Lps22 sensor
    uint8_t bytesReceived = Wire.requestFrom(LPS22_ADDRESS, (uint8_t)3);

    // Check if we received all the bytes we expect
    if (bytesReceived != 3)
    {
        Serial1.print("Error: Expected 3 bytes, received ");
        Serial1.println(bytesReceived);
        return; // Exit if we didn't receive the expected data
    }

    // Read the 3 bytes into the buffer
    for (int i = 0; i < 3; i++)
    {
        if (Wire.available())
        {
            buffer[i] = Wire.read();
        }
        else
        {
            Serial1.println("Error: Data not available!");
            return; // Exit if data is not available
        }
    }

    // Combine bytes into 24-bit signed value
    int32_t raw_pressure = (int32_t)(buffer[2] << 16 | buffer[1] << 8 | buffer[0]);

    *pressure = float(raw_pressure) / 4096;
}

void Lps22::readTemperature(float *temperature)
{
    uint8_t buffer[2];

    // Start I2C transmission and write the register address
    Wire.beginTransmission(LPS22_ADDRESS);
    Wire.write(0x2B); // Start reading from data registers
    if (Wire.endTransmission(false) != 0)
    { // Check if transmission failed
        Serial1.println("Error: Failed to write to Lps22!");
        return; // Exit if transmission failed
    }

    // Request 2 bytes of data from the Lps22 sensor
    uint8_t bytesReceived = Wire.requestFrom(LPS22_ADDRESS, (uint8_t)2);

    // Check if we received all the bytes we expect
    if (bytesReceived != 2)
    {
        Serial1.print("Error: Expected 2 bytes, received ");
        Serial1.println(bytesReceived);
        return; // Exit if we didn't receive the expected data
    }

    // Read the 2 bytes into the buffer
    for (int i = 0; i < 2; i++)
    {
        if (Wire.available())
        {
            buffer[i] = Wire.read();
        }
        else
        {
            Serial1.println("Error: Data not available!");
            return; // Exit if data is not available
        }
    }

    // Combine bytes into 16-bit signed value
    int16_t raw_temperature = (int16_t)(buffer[1] << 8 | buffer[0]);

    *temperature = float(raw_temperature) / 100;
}

void Lps22::writeRegister(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(LPS22_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t Lps22::readRegister(uint8_t reg)
{
    Wire.beginTransmission(LPS22_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(LPS22_ADDRESS, (uint8_t)1);
    uint8_t value = 0;
    if (Wire.available())
    {
        value = Wire.read();
    }
    return value;
}

void Lps22::readRegisters(uint8_t reg, uint8_t *buffer, uint8_t len)
{
    // Start I2C transmission and write the register address
    Wire.beginTransmission(LPS22_ADDRESS);
    Wire.write(reg); // Start reading from data registers
    if (Wire.endTransmission(false) != 0)
    { // Check if transmission failed
        Serial1.println("Error: Failed to write to Lps22!");
        return; // Exit if transmission failed
    }

    // Request len bytes of data from the Lps22 sensor
    uint8_t bytesReceived = Wire.requestFrom(LPS22_ADDRESS, len);

    // Check if we received all the bytes we expect
    if (bytesReceived != len)
    {
        Serial1.print("Error: Expected ");
        Serial1.print(len);
        Serial1.print(" bytes, received ");
        Serial1.println(bytesReceived);
        return; // Exit if we didn't receive the expected data
    }

    // Read the len bytes into the buffer
    for (int i = 0; i < len; i++)
    {
        if (Wire.available())
        {
            buffer[i] = Wire.read();
        }
        else
        {
            Serial1.println("Error: Data not available!");
            return; // Exit if data is not available
        }
    }
}
