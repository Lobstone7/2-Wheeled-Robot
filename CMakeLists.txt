cmake_minimum_required(VERSION 3.22)

project(Robot LANGUAGES C ASM)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE

    core/startup.c
    core/main.c

    drivers/clock.c
    drivers/board.c
    drivers/gpio.c
    drivers/tim.c
    drivers/pwm.c
    drivers/motor.c
    drivers/usart.c
    drivers/exti.c
    drivers/encoder.c
    drivers/prin.c
    drivers/i2c.c
    drivers/imu.c
    drivers/SysTick.c
    drivers/C_Buffer.c
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    core
    drivers
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -Wall
    -Wextra
    -g
)

target_link_options(${PROJECT_NAME}
    PRIVATE
    -T${CMAKE_SOURCE_DIR}/core/linker.ld
    -mcpu=cortex-m4
    -mthumb
    -nostartfiles
    -Wl,-Map=${PROJECT_NAME}.map
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    m
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary
            $<TARGET_FILE:${PROJECT_NAME}>
            ${PROJECT_NAME}.bin

    COMMAND ${CMAKE_OBJCOPY} -O ihex
            $<TARGET_FILE:${PROJECT_NAME}>
            ${PROJECT_NAME}.hex

    COMMAND ${CMAKE_SIZE}
            $<TARGET_FILE:${PROJECT_NAME}>
)