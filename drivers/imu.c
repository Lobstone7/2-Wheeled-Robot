#include "imu.h"

static bool reg_check_helper(I2C_TypeDef* I2C,IMU_Register reg,uint8_t val){
    i2c_write(I2C,IMU_ADDR,reg,val);
    uint8_t check = i2c_read(I2C,IMU_ADDR,reg);
    return check==val;
}

static inline int16_t combine_bytes(uint8_t high, uint8_t low){
    return (int16_t)(((uint16_t)high << 8) | low);
}

bool imu_init(I2C_TypeDef* I2C){
    uint8_t check = i2c_read(I2C,IMU_ADDR,WHO_AM_I);
    if(check != 0x70) return false;

    if(!reg_check_helper(I2C,PWR_MGMT_1,0x00)) return false;
    if(!reg_check_helper(I2C,CONFIG,0x03)) return false;
    if(!reg_check_helper(I2C,GYRO_CONFIG,0x00)) return false;
    if(!reg_check_helper(I2C,ACCEL_CONFIG,0x00)) return false;

    return true;
}

IMU_Data imu_read_raw(I2C_TypeDef* I2C){
    uint8_t buffer[14];
    i2c_read_bytes(I2C,IMU_ADDR,ACCEL_XOUT_H,buffer,14);

    IMU_Data data = {
        .accel = {
            .x = combine_bytes(buffer[0], buffer[1]),
            .y = combine_bytes(buffer[2], buffer[3]),
            .z = combine_bytes(buffer[4], buffer[5]),
        },

        .gyro = {
            .x = combine_bytes(buffer[8], buffer[9]),
            .y = combine_bytes(buffer[10], buffer[11]),
            .z = combine_bytes(buffer[12], buffer[13]),
        },
    };

    return data;
}

GYRO_Bias imu_calibrate(I2C_TypeDef* I2C){
    int32_t sum_x = 0;
    int32_t sum_y = 0;
    int32_t sum_z = 0;

    for(int i = 0;i<1000;i++){
        IMU_Data data = imu_read_raw(I2C);
        sum_x += data.gyro.x;
        sum_y += data.gyro.y;
        sum_z += data.gyro.z;
    }

    GYRO_Bias bias = {
        .x = (float)sum_x / 1000.0f,
        .y = (float)sum_y / 1000.0f,
        .z = (float)sum_z / 1000.0f,
    };

    return bias;
}

IMU_DataF imu_read(I2C_TypeDef *I2C, GYRO_Bias *bias){
    IMU_Data raw = imu_read_raw(I2C);
    IMU_DataF data;

    data.accel.x = (float)raw.accel.x / 16384.0f;
    data.accel.y = (float)raw.accel.y / 16384.0f;
    data.accel.z = (float)raw.accel.z / 16384.0f;

    data.gyro.x = (((float)raw.gyro.x - bias->x) / 131.0f) * DEG_TO_RAD;
    data.gyro.y = (((float)raw.gyro.y - bias->y) / 131.0f) * DEG_TO_RAD;
    data.gyro.z = (((float)raw.gyro.z - bias->z) / 131.0f) * DEG_TO_RAD;

    return data;
}

