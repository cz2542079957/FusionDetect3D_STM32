#pragma once
#include "stm32f1xx.h"

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC

// 状态
#define LED_OFF 0
#define LED_KEEP_ON_MODE 1
#define LED_DELAY_MODE 2

// led初始化
void led_init();
// led常亮
void led_keep_on();
// led闪烁
void led_delay(uint16_t time);
// led轮询事件
void led_on_time(uint16_t interval);
