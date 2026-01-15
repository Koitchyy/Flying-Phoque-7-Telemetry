#ifndef BILDA_H
#define BILDA_H

#include <Arduino.h>
#include <Servo.h>

class Bilda {
public:
  Bilda();
  void begin(int pin);
  void setExtension(float percentage); // 0.0 - 100.0
  void retract();                      // 0%
  void deploy();                       // 100%

private:
  Servo _servo;
  int servoPin;
};

#endif