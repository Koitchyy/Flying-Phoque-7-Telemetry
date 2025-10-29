#include "StatusIndicator.h"
#include <Arduino.h>

StatusIndicator::StatusIndicator(int redPin, int greenPin, int bluePin) {
  this->redPin = redPin;
  this->greenPin = greenPin;
  this->bluePin = bluePin;
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  currentColor = OFF;
}

void StatusIndicator::flash(Color color, int durationMs) {
  this->durationMs = durationMs;
  currentColor = color;
  switch (color) {
    case RED:
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, HIGH);
      break;
    case GREEN:
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, HIGH);
      break;
    case BLUE:
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, LOW);
      break;
    case ORANGE:
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, HIGH);
      break;
    case WHITE:
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
      break;
    default:
      break;
  }
  delay(durationMs / 2);
  off();
  delay(durationMs / 2);
}

void StatusIndicator::solid(Color color) {
  currentColor = color;
  switch (color)
  {
    case RED:
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, HIGH);
      break;
    case GREEN:
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, HIGH);
      break;
    case BLUE:
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, LOW);
      break;
    case ORANGE:
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, HIGH);
      break;
    case WHITE:
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
      break;
    default:
      break;
  }
}

void StatusIndicator::off() {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, HIGH);
  currentColor = OFF;
}