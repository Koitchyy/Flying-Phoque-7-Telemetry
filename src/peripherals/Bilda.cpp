#include "Bilda.h"

Bilda::Bilda() {}

void Bilda::begin(int servoPin) {
  _servo.attach(servoPin);
  retract();
}

void Bilda::setExtension(float percentage) {
  if (percentage < 0.0)
    percentage = 0.0;
  if (percentage > 100.0)
    percentage = 100.0;

  // Linear mapping: 0% -> 500us, 100% -> 2500us
  // 500 + (percentage * 20.0)
  int us = 500 + (int)(percentage * 20.0);
  _servo.writeMicroseconds(us);
}

void Bilda::retract() { setExtension(0.0); }

void Bilda::deploy() { setExtension(100.0); }
