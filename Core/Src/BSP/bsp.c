#include "bsp.h"

void bsp_init() {
  // 串口通讯初始化
  uart_init();
  // 板载按键1初始化
  key1_init();
  //  led灯初始化
  led_init();
  // 蜂鸣器初始化
  beep_init();
}

void on_time(uint16_t interval) {
  // led
  led_on_time(interval);
  // beep
  beep_on_time(interval);
  // uart
  uart_on_time(interval);
}