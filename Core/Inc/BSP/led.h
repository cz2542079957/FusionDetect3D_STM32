#pragma once
#include "stm32f1xx.h"

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC

#define LED_MAX_STATES 128u
#define LED_STATE_ON 1
#define LED_STATE_OFF 0

// led初始化
void led_init();
// led常亮
void led_keep_on();
// led关闭
void led_keep_off();
// led点亮一段时间
void led_delay(uint16_t time);
// led闪烁
void led_flash(uint16_t num);
void led_flash_with_interval(uint16_t num, uint16_t interval);
// led轮询事件
void led_on_time(uint16_t interval);
