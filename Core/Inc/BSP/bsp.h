#pragma once
#include "key1.h"
#include "servo_motor.h"
// #include "icm20948_api.h"

#include "service_protocol.h"
#include "service_battery.h"
#include "service_motion.h"

// 系统时钟初始化
void SystemClock_Config(void);
void SysTick_Handler(void);

// 硬件初始化
void bsp_init();
