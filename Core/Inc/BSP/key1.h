#pragma once

#include "stm32f1xx.h"

#define KEY1_Pin GPIO_PIN_2
#define KEY1_GPIO_Port GPIOD

//状态
#define KEY1_RELEASE 0
#define KEY1_PRESS 1
#define KEY1_LONG_PRESS 2

//长按时间
#define KEY1_LONG_PRESS_TIME 30

//初始化
void key1_init();
//获取状态
int key1_get_state();
//轮询函数
void key1_on_time(uint16_t interval);