#include "board.h"

Motor left_motor = {
    .timer = LEFT_MOTOR_TIMER,
    .channel = PWM_CHANNEL_1,
    .In1 = {
        .port = LEFT_IN1_PORT,
        .pin = LEFT_IN1_PIN
    },
    .In2 = {
        .port = LEFT_IN2_PORT,
        .pin = LEFT_IN2_PIN
    },
};

Motor right_motor = {
    .timer = RIGHT_MOTOR_TIMER,
    .channel = PWM_CHANNEL_2,
    .In1 = {
        .port = RIGHT_IN1_PORT,
        .pin = RIGHT_IN1_PIN
    },
    .In2 = {
        .port = RIGHT_IN2_PORT,
        .pin = RIGHT_IN2_PIN
    },
};

Encoder_s left_encoder = {
    .C1 = {
        .port = LEFT_MOTOR_C1_PORT,
        .pin = LEFT_MOTOR_C1_PIN
    },
    .C2 = {
        .port = LEFT_MOTOR_C2_PORT,
        .pin = LEFT_MOTOR_C2_PIN
    },
    .previous_state = 0,
    .count = 0,
};

Encoder_s right_encoder = {
    .C1 = {
        .port = RIGHT_MOTOR_C1_PORT,
        .pin = RIGHT_MOTOR_C1_PIN
    },
    .C2 = {
        .port = RIGHT_MOTOR_C2_PORT,
        .pin = RIGHT_MOTOR_C2_PIN
    },
    .previous_state = 0,
    .count = 0,
};

void board_init(){
    //clock
    SystemClock_Config();

    //gpio
    rcc_gpioa_enable();
    gpio_init(LEFT_IN1_PORT,LEFT_IN1_PIN,OUTPUT_MODE);
    gpio_init(LEFT_IN2_PORT,LEFT_IN2_PIN,OUTPUT_MODE);

    gpio_init(RIGHT_IN1_PORT,RIGHT_IN1_PIN,OUTPUT_MODE);
    gpio_init(RIGHT_IN2_PORT,RIGHT_IN2_PIN,OUTPUT_MODE);

    gpio_init(TB_STBY_PORT,TB_STBY_PIN,OUTPUT_MODE);

    gpio_init(LEFT_MOTOR_TIMER_PORT,LEFT_MOTOR_TIMER_PIN,ALT_FUNC_MODE);
    gpio_init(RIGHT_MOTOR_TIMER_PORT,RIGHT_MOTOR_TIMER_PIN,ALT_FUNC_MODE);

    gpio_alt(LEFT_MOTOR_TIMER_PORT, LEFT_MOTOR_TIMER_PIN, GPIO_AF1);
    gpio_alt(RIGHT_MOTOR_TIMER_PORT, RIGHT_MOTOR_TIMER_PIN, GPIO_AF1);

    rcc_gpiob_enable();
    gpio_init(RIGHT_MOTOR_C1_PORT,RIGHT_MOTOR_C1_PIN,INPUT_MODE);
    gpio_init(RIGHT_MOTOR_C2_PORT,RIGHT_MOTOR_C2_PIN,INPUT_MODE);

    gpio_init(LEFT_MOTOR_C1_PORT,LEFT_MOTOR_C1_PIN,INPUT_MODE);
    gpio_init(LEFT_MOTOR_C2_PORT,LEFT_MOTOR_C2_PIN,INPUT_MODE);
   
    //tim
    rcc_tim2_enable();
    tim2_5_init(LEFT_MOTOR_TIMER,79,49);
    tim_start(LEFT_MOTOR_TIMER);

    //pwm
    pwm_init(LEFT_MOTOR_TIMER, PWM_CHANNEL_1);
    pwm_init(RIGHT_MOTOR_TIMER, PWM_CHANNEL_2);
    
    //motor
    motor_init(&left_motor);
    motor_init(&right_motor);

    //exti
    rcc_exti_enable();
    exti_init(&left_encoder.C1,BOTH);
    exti_init(&left_encoder.C2,BOTH);

    exti_init(&right_encoder.C1,BOTH);
    exti_init(&right_encoder.C2,BOTH);

    //usart
    rcc_usart_enable();
    usart2_init();

    //encoder
    encoder_init(&left_encoder);
    encoder_init(&right_encoder);

}
