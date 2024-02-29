#pragma once
#include "common.h"

#define STEERING_ENGINE_PORT GPIOC

// #define STEERING_ENGINE_1
#define STEERING_ENGINE_1_PIN GPIO_PIN_3

// #define STEERING_ENGINE_2
#define STEERING_ENGINE_2_PIN GPIO_PIN_2

typedef enum
{
    STEERING_ENGINE_1 = 0,
    STEERING_ENGINE_2 = 1,
    STEERING_ENGINE_NUMS,
} Steering_Engine_ID;

// 初始化
return_code_t steering_engine_init();
// 设置角度
void steering_engine_set_angle(Steering_Engine_ID id, uint16_t angle);
// 设置速度
void steering_engine_set_all_angle(uint16_t angle);

void steering_engine_on_time(uint16_t interval);
