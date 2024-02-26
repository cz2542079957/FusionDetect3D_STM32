
#pragma once
#include "stm32f1xx.h"

#define BEEP_Pin GPIO_PIN_5
#define BEEP_GPIO_Port GPIOC

#define BEEP_MAX_STATES 128u
#define BEEP_STATE_ON 1
#define BEEP_STATE_OFF 0

// 初始化
void beep_init();
// 发出一次声音
void beep_once(int16_t time);
// 发出开启时的声音
void beep_sound_start();
// 发出关闭时的声音
void beep_sound_end();
// 周期轮询函数
void beep_on_time(uint16_t interval);