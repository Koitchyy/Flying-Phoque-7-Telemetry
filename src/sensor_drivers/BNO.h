#ifndef BNO_h
#define BNO_h

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <SparkFun_BNO080_Arduino_Library.h>
#pragma GCC diagnostic pop

class BNO{
    public:
        BNO();
        bool begin();
        bool dataAvailable();
        void getLinearAccelerometer(float* x, float* y, float* z);
        void getRotationVector(float* i, float* j, float* k, float* real);

    private:
        BNO080 bno_;
        uint8_t i2cAddress_;
};

#endif
