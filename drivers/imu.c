#include "imu.h"


typedef void (*FuncHandler) (void* context,  Trans_State state);


static int16_t imu_make_int16(uint8_t high, uint8_t low){
    return (int16_t)(((uint16_t)high << 8) | low);
}

static IMU_Context imu_context;
static IMU_STATE imu_state;
static IMU_DataF dataf;
static PID_Config config = {
    .Kp = 1.0f,
    .Ki = 0.0f,
    .Kd = 0.0f,
    .integral_limit = 0,
    .output_limit = 100,
};

static PID_State p_state = { 
    .integral = 0.0f, 
    .desired_angle = 0.0f 
};


static int32_t gyro_sum_x;
static int32_t gyro_sum_y;
static int32_t gyro_sum_z;
static uint16_t calibration_samples;


static FuncHandler imu_handlers[IMU_PHASE_COUNT] = {
    [IMU_WHO_AM_I] = imu_who_am_i_handler,
    [IMU_PWR_MGMT_1] = imu_pwr_mgmt_1_handler,
    [IMU_CONFIG] = imu_config_handler,
    [IMU_GYRO_CONFIG] = imu_gyro_config_handler,
    [IMU_ACCEL_CONFIG] = imu_accel_config_handler,
    [IMU_VERIFY_PWR_MGMT_1] = imu_verify_pwr_mgmt_1_handler,
    [IMU_VERIFY_CONFIG] = imu_verify_config_handler,
    [IMU_VERIFY_GYRO_CONFIG] = imu_verify_gyro_config_handler,
    [IMU_VERIFY_ACCEL_CONFIG] = imu_verify_accel_config_handler,
    [IMU_CALIBRATE] = imu_calibration_handler,
    [IMU_READ] = imu_read_handler,
    [IMU_DONE] = imu_done_handler,
    [IMU_ERROR] = imu_error_handler,
};

void imu_set_complete_callback(IMU_CompleteCallback callback, void *context){
    imu_context.complete_callback = callback;
    imu_context.complete_context = context;
}


void imu_i2c_callback(Trans_State state, void *context){
    IMU_Context *ctx = context;

    imu_handlers[ctx->phase](ctx, state);
}

void imu_error_handler(void *context, Trans_State state){
    IMU_Context *ctx = (IMU_Context *)context;

    // Error has already been recorded by imu_set_error().
    // Terminal state: do nothing for now.
}

void imu_done_handler(void *context, Trans_State state){
    IMU_Context *ctx = (IMU_Context *)context;

    IMU_Operation operation = ctx->operation;

    ctx->state = state;
    ctx->operation = IMU_OP_NONE;
    ctx->phase = IMU_IDLE;
    
    
    if (ctx->complete_callback != NULL) {
        ctx->complete_callback(operation,state,ctx->complete_context);
    }   
    
}

static void imu_set_error(IMU_Context *ctx, Trans_State state){
    IMU_Operation operation = ctx->operation;

    ctx->error_phase = ctx->phase;
    ctx->state = state;
    ctx->phase = IMU_ERROR;
    ctx->operation = IMU_OP_NONE;
    
    if(ctx->complete_callback != NULL){
        ctx->complete_callback(operation,state,ctx->complete_context);
    }
}


void imu_who_am_i_handler(void *context, Trans_State state){
    IMU_Context *ctx = (IMU_Context *)context;
    if(state != SUCCESS){
       imu_set_error(ctx, state);
        return;
    }
    
    if(ctx->buffer[0] != IMU_WHO_AM_I_VAL){
       imu_set_error(ctx, state);
        return;
    }

    ctx->result = ACTIVE;
    ctx->operation = IMU_OP_INIT;
    ctx->phase = IMU_PWR_MGMT_1;
    i2c_write(I2C1,IMU_ADDR,PWR_MGMT_1,0x00,imu_i2c_callback,ctx);
    
}

void imu_pwr_mgmt_1_handler(void *context, Trans_State state){
    IMU_Context *ctx = (IMU_Context *)context;
    if(state != SUCCESS){
        imu_set_error(ctx, state);
        return;
    }
    
   ctx->phase = IMU_CONFIG;
   i2c_write(I2C1,IMU_ADDR,CONFIG,0x03,imu_i2c_callback,ctx);
}

void imu_config_handler(void *context, Trans_State state){
    IMU_Context *ctx = (IMU_Context *)context;
    if(state != SUCCESS){
       imu_set_error(ctx, state);
        return;
    }
    
   ctx->phase = IMU_GYRO_CONFIG;
   i2c_write(I2C1,IMU_ADDR,GYRO_CONFIG,0x00,imu_i2c_callback,ctx);
}

void imu_gyro_config_handler(void *context, Trans_State state){
    IMU_Context *ctx = (IMU_Context *)context;
    if(state != SUCCESS){
       imu_set_error(ctx, state);
        return;
    }
    
   ctx->phase = IMU_ACCEL_CONFIG;
   i2c_write(I2C1,IMU_ADDR,ACCEL_CONFIG,0x00,imu_i2c_callback,ctx);
}

void imu_accel_config_handler(void *context, Trans_State state){
    IMU_Context *ctx = (IMU_Context *)context;
    if(state != SUCCESS){
        imu_set_error(ctx, state);
        return;
    }
    
   ctx->phase = IMU_VERIFY_PWR_MGMT_1;
   i2c_read_bytes(I2C1,IMU_ADDR,PWR_MGMT_1,ctx->buffer,1,imu_i2c_callback,ctx);
}

void imu_verify_pwr_mgmt_1_handler(void *context, Trans_State state){
    IMU_Context *ctx = (IMU_Context *)context;
    if(state != SUCCESS){
       imu_set_error(ctx, state);
        return;
    }

    if(ctx->buffer[0] != 0x00){
        imu_set_error(ctx, state);
        return;
    }
    
   ctx->phase = IMU_VERIFY_CONFIG;
   i2c_read_bytes(I2C1,IMU_ADDR,CONFIG,ctx->buffer,1,imu_i2c_callback,ctx);
}

void imu_verify_config_handler(void *context, Trans_State state){
    IMU_Context *ctx = (IMU_Context *)context;
    if(state != SUCCESS){
        imu_set_error(ctx, state);
        return;
    }

    if(ctx->buffer[0] != 0x03){
        imu_set_error(ctx, state);
        return;
    }
    
   ctx->phase = IMU_VERIFY_GYRO_CONFIG;
   i2c_read_bytes(I2C1,IMU_ADDR,GYRO_CONFIG,ctx->buffer,1,imu_i2c_callback,ctx);
}

void imu_verify_gyro_config_handler(void *context, Trans_State state){
    IMU_Context *ctx = (IMU_Context *)context;
    if(state != SUCCESS){
        imu_set_error(ctx, state);
        return;
    }

    if(ctx->buffer[0] != 0x00){
        imu_set_error(ctx, state);
        return;
    }
    
   ctx->phase = IMU_VERIFY_ACCEL_CONFIG;
   i2c_read_bytes(I2C1,IMU_ADDR,ACCEL_CONFIG,ctx->buffer,1,imu_i2c_callback,ctx);
}

void imu_verify_accel_config_handler(void *context, Trans_State state){
    IMU_Context *ctx = (IMU_Context *)context;
    if(state != SUCCESS){
       imu_set_error(ctx, state);
        return;
    }

    if(ctx->buffer[0] != 0x00){
        imu_set_error(ctx, state);
        return;
    }
    
    ctx->operation = IMU_OP_NONE;
    ctx->phase = IMU_DONE;
    imu_done_handler(ctx, SUCCESS);
}

void imu_calibration_handler(void *context, Trans_State state){
    IMU_Context *ctx = (IMU_Context *)context;
    if(state != SUCCESS){
       imu_set_error(ctx, state);
        return;
    }
    ctx->result = ACTIVE;
    ctx->operation = IMU_OP_CALIBRATE;

    int16_t gyro_x = imu_make_int16(ctx->buffer[8], ctx->buffer[9]);
    int16_t gyro_y = imu_make_int16(ctx->buffer[10], ctx->buffer[11]);
    int16_t gyro_z = imu_make_int16(ctx->buffer[12], ctx->buffer[13]);

    gyro_sum_x += gyro_x;
    gyro_sum_y += gyro_y;
    gyro_sum_z += gyro_z;

    calibration_samples++;

    if (calibration_samples < IMU_CALIBRATION_SAMPLES) {
        i2c_read_bytes(
            I2C1,
            IMU_ADDR,
            ACCEL_XOUT_H,
            ctx->buffer,
            14,
            imu_i2c_callback,
            ctx
        );

        return;
    }

    imu_state.bias.x = (float)gyro_sum_x / IMU_CALIBRATION_SAMPLES;
    imu_state.bias.y = (float)gyro_sum_y / IMU_CALIBRATION_SAMPLES;
    imu_state.bias.z = (float)gyro_sum_z / IMU_CALIBRATION_SAMPLES;

    ctx->operation = IMU_OP_NONE;
    ctx->phase = IMU_DONE;
    imu_done_handler(ctx, SUCCESS);
    
}

void imu_read_handler(void *context, Trans_State state){
    IMU_Context *ctx = (IMU_Context *)context;
    if(state != SUCCESS){
       imu_set_error(ctx, state);
        return;
    }

    ctx->result = ACTIVE;
    ctx->operation = IMU_OP_READ;

   dataf.accel.x = (float)imu_make_int16(ctx->buffer[0], ctx->buffer[1]) / 16384.0f;
   dataf.accel.y = (float)imu_make_int16(ctx->buffer[2], ctx->buffer[3]) / 16384.0f;
   dataf.accel.z = (float)imu_make_int16(ctx->buffer[4], ctx->buffer[5]) / 16384.0f;

   dataf.gyro.x = (((float)imu_make_int16(ctx->buffer[8], ctx->buffer[9]) - imu_state.bias.x) / 131.0f) * DEG_TO_RAD;
   dataf.gyro.y = (((float)imu_make_int16(ctx->buffer[10], ctx->buffer[11]) - imu_state.bias.y) / 131.0f) * DEG_TO_RAD;
   dataf.gyro.z = (((float)imu_make_int16(ctx->buffer[12], ctx->buffer[13]) - imu_state.bias.z) / 131.0f) * DEG_TO_RAD;

   ctx->operation = IMU_OP_NONE;
   ctx->phase = IMU_DONE;
   imu_done_handler(ctx, SUCCESS);
}


void imu_init(void){
    IMU_Context *ctx = &imu_context;

    ctx->operation = IMU_OP_INIT;
    ctx->phase = IMU_WHO_AM_I;
    i2c_read_bytes(I2C1,IMU_ADDR,WHO_AM_I,ctx->buffer,1,imu_i2c_callback,ctx);
}

void imu_calibrate(void){
    IMU_Context *ctx = &imu_context;

    ctx->operation = IMU_OP_CALIBRATE;
    ctx->phase = IMU_CALIBRATE;

    calibration_samples = 0;
    gyro_sum_x  = 0;
    gyro_sum_y  = 0;
    gyro_sum_z  = 0;

    i2c_read_bytes(I2C1,IMU_ADDR,ACCEL_XOUT_H,ctx->buffer,14,imu_i2c_callback,ctx);
}

void imu_read(void){
    IMU_Context *ctx = &imu_context;

    ctx->operation = IMU_OP_READ;
    ctx->phase = IMU_READ;
    i2c_read_bytes(I2C1,IMU_ADDR,ACCEL_XOUT_H,ctx->buffer,14,imu_i2c_callback,ctx);
}

int imu_pid(void){
    imu_state.gyro_x = dataf.gyro.x;

    float accel_pitch = atan2f(dataf.accel.x,dataf.accel.z);
    
    float dt = 0.010f;
    float gyro_prediction = imu_state.pitch + dataf.gyro.x * dt;
    imu_state.pitch = IMU_ALPHA * gyro_prediction + (1 - IMU_ALPHA) * accel_pitch;

    float error = imu_state.pitch - p_state.desired_angle;
    p_state.integral += error * dt;
    if(p_state.integral >= config.integral_limit){
        p_state.integral = config.integral_limit;
    }
    else if(p_state.integral <= (-config.integral_limit)){
        p_state.integral = -config.integral_limit;
    }

    float proportion = config.Kp * error;
    float integral = config.Ki * p_state.integral;
    float derivative = -config.Kd * dataf.gyro.x;

    float result = proportion + integral + derivative;

    if(result >= config.output_limit){
        result = config.output_limit;
    }
    else if(result <= (-config.output_limit)){
        result = -config.output_limit;
    }

    return (int)result;

}


/*
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

