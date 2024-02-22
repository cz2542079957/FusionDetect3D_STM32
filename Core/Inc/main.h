/*
 * @Author: cz2542079957 2542079957@qq.com
 * @Date: 2024-02-09 15:56:01
 * @LastEditors: cz2542079957 2542079957@qq.com
 * @LastEditTime: 2024-02-22 14:54:19
 * @FilePath: /master/Core/Inc/main.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once

#include <stdio.h>
#include "bsp.h"
#include "stm32f1xx.h"

#define MAIN_LOOP_CYCLE 5                    // 主循环周期
#define SUB_LOOP_CYCLE (MAIN_LOOP_CYCLE * 2) // 子循环周期

void SystemClock_Config(void);
void SysTick_Handler(void);
void Error_Handler(void);