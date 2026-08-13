cmake_minimum_required(VERSION 3.22)

project(Robot LANGUAGES C ASM)

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config
    SYSTEM
    INTERFACE
    ${CMAKE_SOURCE_DIR}/include
)

set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)
set(FREERTOS_PORT "GCC_ARM_CM4F" CACHE STRING "" FORCE)

add_subdirectory(
    ${CMAKE_SOURCE_DIR}/external/FreeRTOS-Kernel
)

target_compile_options(freertos_kernel_port
    PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

target_compile_options(freertos_kernel
    PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE

    core/startup.c
    core/rtos.c
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
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -Wall
    -Wextra
    -g
)

target_link_options(${PROJECT_NAME}
PRIVATE
    -T${CMAKE_SOURCE_DIR}/core/linker.ld
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -nostartfiles
    -Wl,-Map=${PROJECT_NAME}.map
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    m
    freertos_kernel
    freertos_kernel_port
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