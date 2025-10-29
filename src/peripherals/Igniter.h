#ifndef IGNITER_H
#define IGNITER_H

class Igniter {
    public:
        Igniter(int igniterPin, int sensePin);
        void arm();
        void disarm();
        void fire();
        void stop();
        bool isArmed();
        bool isFiring();
        bool igniterCheck();
        int readSensePin();

    private:
        int igniterPin;
        int sensePin;
        bool armed;
        bool firing;
        const int CONNECTION_THRESHOLD = 512;

};

#endif