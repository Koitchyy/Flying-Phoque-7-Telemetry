#ifndef STATES_H
#define STATES_H

enum States {
  BOOT,
  SENSOR_ERROR,
  IDLE,
  AIRBRAKE_TEST,
  IGNITION,
  ASCENT,
  APOGEE,
  DESCENT,
  LANDED
};

const char *stateToString(States state);

#endif