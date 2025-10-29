#ifndef Lps22_h
#define Lps22_h

#include <Wire.h>

class Lps22
{
public:
  Lps22(uint8_t address);
  bool begin();
  void readPressure(float *pressure);
  void readTemperature(float *temperature);

private:
  void writeRegister(uint8_t reg, uint8_t value);
  uint8_t readRegister(uint8_t reg);
  void readRegisters(uint8_t reg, uint8_t *buffer, uint8_t len);
  uint8_t LPS22_ADDRESS;
};

#endif