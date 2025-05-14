#ifndef __MPU6050_H
#define __MPU6050_H

#include "ch32v30x.h"

#define MPU6050_ADDR         0xD0    // I2CµØÖ·
#define RAD_TO_DEG           57.295779513082320876f

// MPU6050¼Ä´æÆ÷¶¨Òå
#define MPU6050_RA_WHO_AM_I         0x75
#define MPU6050_RA_PWR_MGMT_1       0x6B
#define MPU6050_RA_GYRO_CONFIG      0x1B
#define MPU6050_RA_ACCEL_CONFIG     0x1C
#define MPU6050_RA_CONFIG          0x1A
#define MPU6050_RA_SMPLRT_DIV      0x19
#define MP

#endif 