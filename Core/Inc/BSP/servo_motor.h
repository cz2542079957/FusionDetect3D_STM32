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

// 初始化
return_code_t servo_motor_init();
// 设置角度
void servo_motor_set_angle(SERVO_MOTOR_ID id, float angle);
// 设置速度
void servo_motor_set_all_angle(float angle);

void servo_motor_on_time(uint16_t interval);
