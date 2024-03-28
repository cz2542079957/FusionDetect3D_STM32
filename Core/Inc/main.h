/*
 * @Author: cz2542079957 2542079957@qq.com
 * @Date: 2024-02-09 15:56:01
 * @LastEditors: cz2542079957 2542079957@qq.com
 * @LastEditTime: 2024-03-28 10:17:54
 * @FilePath: /master/Core/Inc/main.h
 * @Description:
 *
 * Copyright (c) 2024 by 2542079957@qq.com, All Rights Reserved.
 */

#pragma once
#include "bsp.h"

/**
 * @description: 四级轮询函数
 *      1.一般而言，越快的轮询中执行各种器件或者服务的on_time函数时，精度也会越高
 *      2.同时，大约每1次main轮询周期，会产生1ms的误差，因此在延迟函数中要减一，达到缓解误差的目的
 */
#define MAIN_LOOP_CYCLE 5             // 主循环周期(MS)
#define FAST_LOOP_CYCLE_TICKS 1       // 快速循环周期(主周期数)
#define NORMAL_LOOP_CYCLE_TICKS 2     // 普通循环周期(主周期数)
#define SLOW_LOOP_CYCLE_TICKS 20      // 慢速循环周期(主周期数)
#define SLOWEST_LOOP_CYCLE_TICKS 1000 // 最慢循环周期(主周期数)
                                      // 快周期轮询
void fast_on_time(uint16_t interval);
// 一般周期轮询
void normal_on_time(uint16_t interval);
// 慢周期轮询
void slow_on_time(uint16_t interval);
// 特慢周期轮询
void slowest_on_time(uint16_t interval);
