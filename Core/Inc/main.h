#pragma once

#include "bsp.h"
#include "stm32f1xx_hal.h"

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

void Error_Handler(void);

#define ON_TIME_INTERVAL 10 // 周期轮询时间间隔




