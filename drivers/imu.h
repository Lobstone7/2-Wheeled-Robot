#ifndef IMU_H
#define IMU_H

#include "common.h"
#include "SysTick.h"
#include "i2c.h"
#define IMU_ADDR 0x68

#define IMU_RX_BUFFER_SIZE 14
#define IMU_WHO_AM_I_VAL 0x70U
#define IMU_CALIBRATION_SAMPLES 1000U

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

typedef enum {
    IMU_IDLE,
    IMU_WHO_AM_I,
    IMU_PWR_MGMT_1,
    IMU_CONFIG,
    IMU_GYRO_CONFIG,
    IMU_ACCEL_CONFIG,
    IMU_VERIFY_PWR_MGMT_1,
    IMU_VERIFY_CONFIG,
    IMU_VERIFY_GYRO_CONFIG,
    IMU_VERIFY_ACCEL_CONFIG,
    IMU_CALIBRATE,
    IMU_READ,
    IMU_DONE,
    IMU_ERROR,
    IMU_PHASE_COUNT,
}IMU_Phase;

typedef struct {
    I2C_TypeDef *I2C;
    IMU_Phase phase;
    IMU_Phase error_phase;
    uint8_t buffer[IMU_RX_BUFFER_SIZE];
} IMU_Context;



void imu_i2c_callback(Trans_State result, void *context);

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

void imu_i2c_callback(Trans_State state, void *context);

void imu_who_am_i_handler(void *context, Trans_State state);
void imu_pwr_mgmt_1_handler(void *context, Trans_State state);
void imu_config_handler(void *context, Trans_State state);
void imu_gyro_config_handler(void *context, Trans_State state);
void imu_accel_config_handler(void *context, Trans_State state);

void imu_verify_pwr_mgmt_1_handler(void *context, Trans_State state);
void imu_verify_config_handler(void *context, Trans_State state);
void imu_verify_gyro_config_handler(void *context, Trans_State state);
void imu_verify_accel_config_handler(void *context, Trans_State state);

void imu_calibration_handler(void *context, Trans_State state);
void imu_read_handler(void *context, Trans_State state);

void imu_done_handler(void *context, Trans_State state);
void imu_error_handler(void *context, Trans_State state);

void imu_init(void);
void imu_calibrate(void);
void imu_read(void);


/*
Function_Result imu_init(I2C_TypeDef *I2C);
IMU_Data imu_read_raw(I2C_TypeDef* I2C);
GYRO_Bias imu_calibrate(I2C_TypeDef* I2C);
IMU_DataF imu_read(I2C_TypeDef *I2C, GYRO_Bias *bias);
void imu_update(I2C_TypeDef *I2C, IMU_STATE *state);
int PID(PID_Config *config, PID_State *p_state, float pitch, float dt, float gyro);
uint8_t return_buffer();*/

#endif