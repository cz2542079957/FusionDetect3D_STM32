#pragma once
#include "common.h"

#define SERVO_MOTOR_PORT GPIOC
#define SERVO_MOTOR_1_PIN GPIO_PIN_3
#define SERVO_MOTOR_2_PIN GPIO_PIN_2

typedef enum
{
    SERVO_MOTOR_1 = 0,
    SERVO_MOTOR_2 = 1,
    SERVO_MOTOR_NUMS,
} SERVO_MOTOR_ID;

extern float servo_motor_pwm_width[SERVO_MOTOR_NUMS]; //  舵机pwm占空比当前要达到的值

// 初始化
return_code_t servo_motor_init();
