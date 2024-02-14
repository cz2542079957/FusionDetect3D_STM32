#pragma once
#include "stm32f1xx.h"

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC

// led初始化
void led_init();
// led常亮
void led_keep_on();
// led点亮一段时间
void led_delay(uint16_t time);
// led闪烁
void led_flash(uint16_t num);
void led_flash_with_interval(uint16_t num, uint16_t interval);
// led轮询事件
void led_on_time(uint16_t interval);
