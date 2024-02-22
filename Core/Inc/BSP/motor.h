
#pragma once
#include "common.h"

#define MOTOR_ENABLE_A (0x01)
#define MOTOR_ENABLE_B (0x02)
#define MOTOR_ENABLE_C (0x04)
#define MOTOR_ENABLE_D (0x08)

/* GPIO */
#define M1A_PORT GPIOA
#define M1A_PIN GPIO_PIN_11
#define M1B_PORT GPIOA
#define M1B_PIN GPIO_PIN_8

#define M2A_PORT GPIOB
#define M2A_PIN GPIO_PIN_0
#define M2B_PORT GPIOB
#define M2B_PIN GPIO_PIN_1

#define M3A_PORT GPIOC
#define M3A_PIN GPIO_PIN_6
#define M3B_PORT GPIOC
#define M3B_PIN GPIO_PIN_7

#define M4A_PORT GPIOC
#define M4A_PIN GPIO_PIN_8
#define M4B_PORT GPIOC
#define M4B_PIN GPIO_PIN_9

/* PWM */
#define PWM_M1_A TIM8->CCR1
#define PWM_M1_B TIM8->CCR2

#define PWM_M2_A TIM8->CCR3
#define PWM_M2_B TIM8->CCR4

#define PWM_M3_A TIM1->CCR4
#define PWM_M3_B TIM1->CCR1

#define PWM_M4_A TIM1->CCR2
#define PWM_M4_B TIM1->CCR3

typedef enum
{
    MOTOR_ID_M1 = 0,
    MOTOR_ID_M2,
    MOTOR_ID_M3,
    MOTOR_ID_M4,
    MOTOR_NUMS
} Motor_ID;

// 电机状态
typedef enum
{
    // 电机速度最值
    MOTOR_SPEED_MAX = 3600,
    MOTOR_SPEED_MIN = -3600,
    // 电机死区校正值
    MOTOR_IGNORE_PULSE = 2000,
    // 速度根据时间的变化系数
    MOTOR_SPEED_INCREMENT_COE = 1,
    MOTOR_FRONT = 1,
    MOTOR_BACK = -1,
    MOTOR_STOP = 0
} Motor_Move;

// 初始化
void motor_init();
// 电机停止
void motor_stop();
// 设置电机脉冲(0-1600)
void motor_set_pwm(Motor_ID id, int16_t pulse);
void motor_set_all_pwm(int16_t pulse);
