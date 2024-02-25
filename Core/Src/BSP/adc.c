#include "adc.h"

/* private */
ADC_HandleTypeDef hadc1;
uint16_t adc1_value;

// 中断响应函数
void ADC1_2_IRQHandler(void)
{
    if (__HAL_ADC_GET_FLAG(&hadc1, ADC_FLAG_EOC))
    {
        adc1_value = HAL_ADC_GetValue(&hadc1);
        __HAL_ADC_CLEAR_FLAG(&hadc1, ADC_FLAG_EOC);
    }
}

/* public */
return_code_t adc_init(void)
{
    return_code_t ret = RETURN_OK;
    ret |= adc_adc1_init();
    return ret;
}

return_code_t adc_adc1_init(void)
{
    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PC4     ------> ADC1_IN14
    */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = ADC_ADC1_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(ADC_ADC1_GPIO_PORT, &GPIO_InitStruct);

    ADC_ChannelConfTypeDef sConfig = {0};
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        return RETURN_FAIL;
    }

    // 通道 1,规则采样顺序值为 1,采样时间为 239.5 周期
    sConfig.Channel = ADC_ADC1_CHANNEL;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        return RETURN_FAIL;
    }

    // ADC1中断服务
    HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);

    // 启动ADC并使能中断
    if (HAL_ADC_Start_IT(&hadc1) == HAL_OK)
    {
        return RETURN_FAIL;
    }

    return RETURN_OK;
}

uint16_t adc_adc1_get()
{
    while (HAL_ADC_Start_IT(&hadc1) != HAL_OK)
        ;              // 这样可以防止过于频繁的触发中断
    return adc1_value; // 返回最近一次转换的值
}
