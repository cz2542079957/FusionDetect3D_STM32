#pragma once
#include "adc.h"

// 平均值计算取值周期(ms) (这段时间内的取值计算为平均值)
#define BATTERY_COMPUTE_AVG_INTERVAL 1000

// 电池周期函数，用于计算一段时间内平均电压
void battery_on_time(uint16_t interval);
// 获取最后一次计算的平均电压（mV）(由于不使用浮点数，所以乘以1000表示mV)
uint16_t battery_get_voltage();