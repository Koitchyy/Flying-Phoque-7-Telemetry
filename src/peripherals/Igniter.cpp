#include "Igniter.h"
#include <Arduino.h>

Igniter::Igniter(int igniterPin, int sensePin) {
  this->igniterPin = igniterPin;
  this->sensePin = sensePin;
  armed = false;
  firing = false;
}

void Igniter::arm() {
  armed = true;
}

void Igniter::disarm() {
  armed = false;
}

void Igniter::fire() {
  if (armed) {
    firing = true;
    digitalWrite(igniterPin, HIGH);
  }
}

void Igniter::stop() {
  firing = false;
  digitalWrite(igniterPin, LOW);
}

bool Igniter::isArmed() {
  return armed;
}

bool Igniter::isFiring() {
  return firing;
}

bool Igniter::igniterCheck() {
  return analogRead(sensePin) > CONNECTION_THRESHOLD;
}

int Igniter::readSensePin() {
  return analogRead(sensePin);
}