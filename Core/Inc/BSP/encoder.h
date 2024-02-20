#pragma once
#include "stm32f1xx.h"

#define HAL_1A_PIN GPIO_PIN_0
#define HAL_1A_PORT GPIOA
#define HAL_1B_PIN GPIO_PIN_1
#define HAL_1B_PORT GPIOA

#define HAL_2A_PIN GPIO_PIN_7
#define HAL_2A_PORT GPIOA
#define HAL_2B_PIN GPIO_PIN_6
#define HAL_2B_PORT GPIOA

#define HAL_3A_PIN GPIO_PIN_15
#define HAL_3A_PORT GPIOA
#define HAL_3B_PIN GPIO_PIN_3
#define HAL_3B_PORT GPIOB

#define HAL_4A_PIN GPIO_PIN_7
#define HAL_4A_PORT GPIOB
#define HAL_4B_PIN GPIO_PIN_6
#define HAL_4B_PORT GPIOB

// 不可大于65535 因为F103的定时器是16位的。
#define ENCODER_TIM_PERIOD (uint16_t)(65535)

void Encoder_Init(void);
int Encoder_Get_Count_Now(uint8_t Motor_id);
void Encoder_Update_Count(void);
void Encoder_Get_ALL(int *Encoder_all);
void Encoder_Send_Count_Now(void);
