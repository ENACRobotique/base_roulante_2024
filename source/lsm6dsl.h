#pragma once

typedef struct {
    float gx;
    float gy;
    float gz;
    float xlx;
    float xly;
    float xlz;
} imu_data_t;


void imuStart();
