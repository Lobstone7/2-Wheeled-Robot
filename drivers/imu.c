#include "imu.h"

/*
typedef void (*FuncHandler) (void* context,  Trans_State state);

static FuncHandler imu_handlers[IMU_PHASE_COUNT] = {
    [IMU_WHO_AM_I] = imu_who_am_i_handler,
    [IMU_PWR_MGMT_1] = imu_pwr_mgmt_1_handler,
    [IMU_CONFIG]   = imu_config_handler,
    [IMU_ACCEL]    = imu_accel_handler,
    [IMU_GYRO]    = imu_gyro_handler,
    [IMU_DONE]    = imu_done_handler,
    [IMU_ERROR]    = imu_error_handler,
};

void imu_i2c_callback(Trans_State state, void *context){
    IMU_Context *ctx = context;

    imu_handlers[ctx->phase](ctx, state);
}

void imu_init_who_am_i_handler(void *context, Trans_State state){
    IMU_Context *ctx = (IMU_Context *)context;
    if(state != SUCCESS){
        
    }
    else{
        if(ctx->buffer[0] == IMU_ADDR){
            ctx->phase = IMU_PWR_MGMT_1;
            i2c_write(I2C1,IMU_ADDR,PWR_MGMT_1,0x00,imu_i2c_callback, (void*)ctx);
        }
    }
}


void imu_init(){

}





typedef struct {
    uint8_t raw_buffer[14];
    IMU_Data data;
    bool read_pending;
    IMU_InitPhase init_phase;
} IMU_Context;

static IMU_Context imu_context;

static bool reg_check_helper(I2C_TypeDef* I2C,IMU_Register reg,uint8_t val){
    i2c_write(I2C,IMU_ADDR,reg,val);
    uint8_t check = i2c_read(I2C,IMU_ADDR,reg);
    return check==val;
}

static inline int16_t combine_bytes(uint8_t high, uint8_t low){
    return (int16_t)(((uint16_t)high << 8) | low);
}

Function_Result imu_init(I2C_TypeDef *I2C){
    if (imu_context.init_phase != IMU_INIT_IDLE &&
        imu_context.init_phase != IMU_INIT_DONE &&
        imu_context.init_phase != IMU_INIT_ERROR) {
        return ACTIVE;
    }

    imu_context.init_phase = IMU_INIT_WHO_AM_I;

    return i2c_read_bytes(
        I2C,
        IMU_ADDR,
        WHO_AM_I,
        imu_context.raw_buffer,
        1
    );
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
    float sum_x = 0;
    float sum_y = 0;
    float sum_z = 0;

    for(int i = 0;i<1000;i++){
        IMU_Data data = imu_read_raw(I2C);
        sum_x += data.gyro.x;
        sum_y += data.gyro.y;
        sum_z += data.gyro.z;
    }

    GYRO_Bias bias = {
        .x = sum_x / 1000.0f,
        .y = sum_y / 1000.0f,
        .z = sum_z / 1000.0f,
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

void imu_update(I2C_TypeDef *I2C, IMU_STATE *state){
    IMU_DataF data = imu_read(I2C,&state->bias);
    state->gyro_x = data.gyro.x;

    float accel_pitch = atan2f(data.accel.x,data.accel.z);
    uint32_t current_ms = SysTick_get_ms();

    if (state->previous_ms == 0) {
        state->previous_ms = current_ms;
    }
    float dt = (current_ms - state->previous_ms) / 1000.0f;
    state->previous_ms = current_ms;
    float gyro_prediction = state->pitch + data.gyro.x * dt;
    state->pitch = IMU_ALPHA * gyro_prediction + (1 - IMU_ALPHA) * accel_pitch;

}

int PID(PID_Config *config, PID_State *p_state, float pitch, float dt, float gyro){
    float error = pitch - p_state->desired_angle;
    p_state->integral += error * dt;
    if(p_state->integral >= config->integral_limit){
        p_state->integral = config->integral_limit;
    }
    else if(p_state->integral <= (-config->integral_limit)){
         p_state->integral = -config->integral_limit;
    }

    float proportion = config->Kp * error;
    float integral = config->Ki * p_state->integral;
    float derivative = -config->Kd * gyro;

    float result = proportion + integral + derivative;

    if(result >= config->output_limit){
        result = config->output_limit;
    }
    else if(result <= config->output_limit){
        result = -config->output_limit;
    }

    return (int)result;

}
*/

