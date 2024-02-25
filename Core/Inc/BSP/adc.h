#pragma once
#include "common.h"

/* ADC1 用于电池电压测量 */
#define ADC_ADC1_GPIO_PORT GPIOC
#define ADC_ADC1_GPIO_PIN GPIO_PIN_4
#define ADC_ADC1_CHANNEL ADC_CHANNEL_14

// ADC初始化
return_code_t adc_init(void);
// ADC1初始化
return_code_t adc_adc1_init(void);
// ADC1获取值
uint16_t adc_adc1_get();