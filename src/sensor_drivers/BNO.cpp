#include "BNO.h"

BNO::BNO() : i2cAddress_(0x4A)
{
}

bool BNO::begin()
{
    // Give the BNO time to boot up after power-on or reset
    delay(300);
    
    Serial1.println("Initializing BNO080...");
    
    // BNO085 uses address 0x4A by default
    // Some versions of the library need the address, some don't
    bool initialized = false;
    
    // Try with default address first (no parameter)
    if (bno_.begin())
    {
        initialized = true;
        Serial1.println("BNO080 connected at default address");
    }
    else
    {
        // Try with explicit address 0x4A
        Serial1.println("Trying explicit address 0x4A...");
        if (bno_.begin(i2cAddress_))
        {
            initialized = true;
            Serial1.print("BNO080 connected at 0x");
            Serial1.println(i2cAddress_, HEX);
        }
        else
        {
            Serial1.println("BNO080 failed to initialize!");
            return false;
        }
    }

    // Enable required reports @ 50 Hz (20 ms).  Adjust as needed.
    bno_.enableLinearAccelerometer(5); // ms between reports
    bno_.enableRotationVector(5);
    
    // Give it a moment to start reporting
    delay(150);
    
    Serial1.println("BNO080 fully initialized!");
    return true;
}

bool BNO::dataAvailable()
{
    return bno_.dataAvailable();
}

void BNO::getLinearAccelerometer(float* x, float* y, float* z) {
  if (x) *x = bno_.getLinAccelX();
  if (y) *y = bno_.getLinAccelY();
  if (z) *z = bno_.getLinAccelZ();
}

void BNO::getRotationVector(float* i, float* j, float* k, float* real) {
  if (i)    *i    = bno_.getQuatI();
  if (j)    *j    = bno_.getQuatJ();
  if (k)    *k    = bno_.getQuatK();
  if (real) *real = bno_.getQuatReal();
}