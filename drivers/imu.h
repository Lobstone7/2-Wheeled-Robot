#ifndef IMU_H
#define IMU_H

#include "common.h"
#include "SysTick.h"
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
    float x;
    float y;
    float z;
}GYRO_Bias;

typedef struct{
    float x;
    float y;
    float z;
}IMU_VectorF;

typedef struct{
    IMU_VectorF accel;
    IMU_VectorF gyro;
}IMU_DataF;

typedef struct{
    GYRO_Bias bias;
    float pitch;
    uint32_t previous_ms;
    float gyro_x;
}IMU_STATE;

typedef enum{
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
}IMU_Register;

#define IMU_ALPHA 0.98f

typedef struct{
    float Kp;
    float Ki;
    float Kd;
    float integral_limit;
    float output_limit;
}PID_Config;

typedef struct{
    float integral;
    float desired_angle;
}PID_State;

bool imu_init(I2C_TypeDef* I2C);
IMU_Data imu_read_raw(I2C_TypeDef* I2C);
GYRO_Bias imu_calibrate(I2C_TypeDef* I2C);
IMU_DataF imu_read(I2C_TypeDef *I2C, GYRO_Bias *bias);
void imu_update(I2C_TypeDef *I2C, IMU_STATE *state);
int PID(PID_Config *config, PID_State *p_state, float pitch, float dt, float gyro);

#endif