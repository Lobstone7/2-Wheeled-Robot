#ifndef IMU_H
#define IMU_H

#include "i2c.h"
#define IMU_ADDR 0x68

typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
}IMU_Vector;

typedef struct{
    IMU_Vector accel;
    IMU_Vector gyro;
}IMU_Data;

typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
}GYRO_Bias;

typedef struct
{
    float x;
    float y;
    float z;
} IMU_VectorF;

typedef struct
{
    IMU_VectorF accel;
    IMU_VectorF gyro;
} IMU_DataF;

typedef enum
{
    CONFIG          = 0x1A,
    GYRO_CONFIG     = 0x1B,
    ACCEL_CONFIG    = 0x1C,
    ACCEL_CONFIG_2  = 0x1D,

    ACCEL_XOUT_H    = 0x3B,
    ACCEL_XOUT_L    = 0x3C,
    ACCEL_YOUT_H    = 0x3D,
    ACCEL_YOUT_L    = 0x3E,
    ACCEL_ZOUT_H    = 0x3F,
    ACCEL_ZOUT_L    = 0x40,

    TEMP_OUT_H      = 0x41,
    TEMP_OUT_L      = 0x42,

    GYRO_XOUT_H     = 0x43,
    GYRO_XOUT_L     = 0x44,
    GYRO_YOUT_H     = 0x45,
    GYRO_YOUT_L     = 0x46,
    GYRO_ZOUT_H     = 0x47,
    GYRO_ZOUT_L     = 0x48,

    PWR_MGMT_1      = 0x6B,
    PWR_MGMT_2      = 0x6C,

    WHO_AM_I        = 0x75
} IMU_Register;

bool imu_init(I2C_TypeDef* I2C);
IMU_Data imu_read_raw(I2C_TypeDef* I2C);
GYRO_Bias imu_calibrate(I2C_TypeDef* I2C);
IMU_DataF imu_read(I2C_TypeDef *I2C, GYRO_Bias *bias);

#endif