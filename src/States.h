#ifndef STATES_H
#define STATES_H

enum States
{
    BOOT,
    IDLE,
    IGNITION,
    ASCENT,
    APOGEE,
    DESCENT,
    LANDED
};

const char *stateToString(States state);

#endif