#pragma once
#include "stm32f1xx.h"

#define BEEP_Pin GPIO_PIN_5
#define BEEP_GPIO_Port GPIOC

//// 蜂鸣器启动
// inline void beep_on();
//// 蜂鸣器关闭
// inline void beep_off();



// 发出开启时的声音
void beep_sound_start();
// 发出关闭时的声音
void beep_sound_end();
