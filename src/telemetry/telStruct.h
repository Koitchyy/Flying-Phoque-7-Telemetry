#pragma once
#include <Arduino.h>

struct TelemetryPacket {
    uint32_t time_ms;
    float accel_x, accel_y, accel_z; // ADXL345
    float accel_x_high_g, accel_y_high_g, accel_z_high_g; // ADXL375
    float pressure;
    float temperature;
    float altitude;
    float bno_x, bno_y, bno_z;                 
    float bno_i, bno_j, bno_k, bno_real;
    uint8_t state;
    uint8_t crc;
} __attribute__((packed));

inline uint8_t computeCRC(const uint8_t *data, size_t len) {
    uint8_t crc = 0;
    for (size_t i = 0; i < len; i++) crc ^= data[i];
    return crc;
}
