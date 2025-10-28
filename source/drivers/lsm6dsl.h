#pragma once

typedef struct {
    float gx;
    float gy;
    float gz;
    float xlx;
    float xly;
    float xlz;
} imu_data_t;

#define IMU_DATA_NB 240

extern imu_data_t imu_data_buffer[IMU_DATA_NB];
extern int imu_data_write_index;
extern int imu_data_read_index;

void imuStart();
