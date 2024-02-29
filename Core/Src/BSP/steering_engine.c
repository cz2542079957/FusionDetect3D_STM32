#include "steering_engine.h"

TIM_HandleTypeDef htim7;

uint16_t g_pwm_pulse = 0;
uint8_t g_pwm_angle[STEERING_ENGINE_NUMS] = {0};
uint16_t g_angle_num[STEERING_ENGINE_NUMS] = {0};

void TIM7_IRQHandler(void)
{
    //     if (__HAL_TIM_GET_FLAG(&htim7, TIM_FLAG_UPDATE) != RESET)
    //     {
    //         if (__HAL_TIM_GET_IT_SOURCE(&htim7, TIM_IT_UPDATE) != RESET)
    //         {
    //             __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE); // 清除更新中断标志
    //             steering_engine_on_time(0);
    //             // HAL_TIM_IRQHandler(&htim7);
    //             printf("debug\n");
    //         }
    //     }
    // __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE); // 清除更新中断标志
    // printf("debug\n");
}

return_code_t steering_engine_gpio_init()
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = STEERING_ENGINE_1_PIN | STEERING_ENGINE_2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(STEERING_ENGINE_PORT, &GPIO_InitStruct);
    return RETURN_OK;
}

return_code_t steering_engine_init()
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    __HAL_RCC_TIM7_CLK_ENABLE();
    htim7.Instance = TIM7;
    htim7.Init.Prescaler = 71;
    htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim7.Init.Period = 9;
    htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
    {
        return RETURN_FAIL;
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
    {
        return RETURN_FAIL;
    }

    // TIM7 中断
    HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);

    // GPIO 初始化
    if (steering_engine_gpio_init() != RETURN_OK)
    {
        return RETURN_FAIL;
    }

    return RETURN_OK;
}

void steering_engine_set_angle(Steering_Engine_ID id, uint16_t angle)
{
    if (id == STEERING_ENGINE_1)
    {
        g_pwm_angle[id] = angle;
        g_angle_num[id] = (angle * 11 + 500) / 10;
    }
    else if (id == STEERING_ENGINE_2)
    {
        g_pwm_angle[id] = angle;
        g_angle_num[id] = (angle * 11 + 500) / 10;
    }
}

void steering_engine_set_all_angle(uint16_t angle) {}

void steering_engine_on_time(uint16_t interval)
{
    g_pwm_pulse++;

    if (g_pwm_pulse <= g_angle_num[0])
        HAL_GPIO_WritePin(STEERING_ENGINE_PORT, STEERING_ENGINE_1_PIN, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(STEERING_ENGINE_PORT, STEERING_ENGINE_1_PIN, GPIO_PIN_RESET);

    if (g_pwm_pulse >= 2000)
        g_pwm_pulse = 0;
}
