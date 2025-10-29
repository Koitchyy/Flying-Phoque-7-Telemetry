#include "States.h"

const char *stateToString(States state)
{
    switch (state)
    {
    case BOOT:
        return "BOOT";
    case IDLE:
        return "IDLE";
    case IGNITION:
        return "IGNITION";
    case ASCENT:
        return "ASCENT";
    case APOGEE:
        return "APOGEE";
    case DESCENT:
        return "DESCENT";
    case LANDED:
        return "LANDED";
    default:
        return "UNKNOWN";
    }
}