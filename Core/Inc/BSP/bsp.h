#pragma once
#include "beep.h"
#include "led.h"
#include "uart.h"

void bsp_init();

// 周期轮询
void on_time(uint16_t interval);