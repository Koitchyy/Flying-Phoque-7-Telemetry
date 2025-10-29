#ifndef Adxl_h
#define Adxl_h

#include <Wire.h>

enum ADXL_TYPE
{
  ADXL345,
  ADXL375
};

class Adxl
{
public:
  Adxl(uint8_t address, ADXL_TYPE adxlType);
  bool begin();
  void readAccelerometer(float *x, float *y, float *z);

private:
  // TwoWire Wire;
  void writeRegister(uint8_t reg, uint8_t value);
  uint8_t readRegister(uint8_t reg);
  void readRegisters(uint8_t reg, uint8_t *buffer, uint8_t len);
  uint8_t ADXL_ADDRESS;
  ADXL_TYPE adxlType;
  float g_per_LSB;
};

#endif