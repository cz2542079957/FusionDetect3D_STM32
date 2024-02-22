#pragma once
#include "common.h"

#define TIM2_CH1_PORT1 GPIOA
#define TIM2_CH1_PIN GPIO_PIN_15
#define TIM2_CH2_PORT2 GPIOB
#define TIM2_CH2_PIN GPIO_PIN_3

#define TIM3_CHx_PORT GPIOA
#define TIM3_CH1_PIN GPIO_PIN_6
#define TIM3_CH2_PIN GPIO_PIN_7

#define TIM4_CHx_PORT GPIOB
#define TIM4_CH1_PIN GPIO_PIN_6
#define TIM4_CH2_PIN GPIO_PIN_7

#define TIM5_CHx_PORT GPIOA
#define TIM5_CH1_PIN GPIO_PIN_0
#define TIM5_CH2_PIN GPIO_PIN_1

struct encoder_count_t
{
    int32_t encoder_tim2_count;
    int32_t encoder_tim3_count;
    int32_t encoder_tim4_count;
    int32_t encoder_tim5_count;
};

// 不可大于65535 因为F103的定时器是16位的。
#define ENCODER_TIM_PERIOD (uint16_t)(65535)

// 初始化
void encoder_init();
// 更新计数
void encoder_update_count();

void encoder_on_time(uint16_t interval);