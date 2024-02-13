#pragma once
#include "stm32f1xx.h"

#define BEEP_Pin GPIO_PIN_5
#define BEEP_GPIO_Port GPIOC 

void beep_init();
// 发出开启时的声音
void beep_sound_start();
// 发出关闭时的声音
void beep_sound_end();
// 周期轮询函数
void beep_on_time(uint16_t interval);