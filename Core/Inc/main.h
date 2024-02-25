/*
 * @Author: cz2542079957 2542079957@qq.com
 * @Date: 2024-02-09 15:56:01
 * @LastEditors: cz2542079957 2542079957@qq.com
 * @LastEditTime: 2024-02-25 12:56:36
 * @FilePath: /master/Core/Inc/main.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once

#include <stdio.h>
#include "bsp.h"
#include "stm32f1xx.h"

#define MAIN_LOOP_CYCLE 5             // 主循环周期(MS)
#define FAST_LOOP_CYCLE_TICKS 1       // 快速循环周期(主周期数)
#define NORMAL_LOOP_CYCLE_TICKS 2     // 普通循环周期(主周期数)
#define SLOW_LOOP_CYCLE_TICKS 20      // 慢速循环周期(主周期数)
#define SLOWEST_LOOP_CYCLE_TICKS 1000 // 最慢循环周期(主周期数)

void SystemClock_Config(void);
void SysTick_Handler(void);
void Error_Handler(void);