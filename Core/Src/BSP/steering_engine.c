#include "steering_engine.h"

TIM_HandleTypeDef htim6;

uint16_t g_pwm_pulse = 0;
uint8_t g_pwm_angle[STEERING_ENGINE_NUMS] = {0};
uint16_t g_angle_num[STEERING_ENGINE_NUMS] = {0};

void TIM6_IRQHandler(void)
{
    // printf("debug\n");
    // if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)
    // {
    //     if (__HAL_TIM_GET_IT_SOURCE(&htim6, TIM_IT_UPDATE) != RESET)
    //     {
    //         __HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);
    //         steering_engine_on_time(0);
    //         __HAL_TIM_ENABLE_IT(&htim6, TIM_IT_UPDATE); // 添加这行来重新使能中断
    //     }
    // }
    __HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE); // 清除更新中断标志
    steering_engine_on_time(0);
    // __HAL_TIM_ENABLE_IT(&htim6, TIM_IT_UPDATE); // 添加这行来重新使能中断
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

    // TIM6 中断
    __HAL_RCC_TIM6_CLK_ENABLE();
    htim6.Instance = TIM6;
    htim6.Init.Prescaler = 71;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim6.Init.Period = 9;
    htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
    {
        return RETURN_FAIL;
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
    {
        return RETURN_FAIL;
    }
    HAL_NVIC_SetPriority(TIM6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM6_IRQn);
    __HAL_TIM_ENABLE_IT(&htim6, TIM_IT_UPDATE);
    __HAL_TIM_ENABLE(&htim6);

    // GPIO 初始化
    if (steering_engine_gpio_init() != RETURN_OK)
    {
        return RETURN_FAIL;
    }

    return RETURN_OK;
}

void steering_engine_set_angle(Steering_Engine_ID id, uint16_t angle)
{
    g_pwm_angle[id] = angle;
    g_angle_num[id] = (angle * 11 + 500) / 10;
    printf("%u\n", g_angle_num[id]);
}

void steering_engine_set_all_angle(uint16_t angle) {}

void steering_engine_on_time(uint16_t interval)
{
    g_pwm_pulse += 1;
    // printf("%u\n", g_pwm_pulse);
    if (g_pwm_pulse <= g_angle_num[STEERING_ENGINE_1])
        HAL_GPIO_WritePin(STEERING_ENGINE_PORT, STEERING_ENGINE_1_PIN, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(STEERING_ENGINE_PORT, STEERING_ENGINE_1_PIN, GPIO_PIN_RESET);

    if (g_pwm_pulse <= g_angle_num[STEERING_ENGINE_2])
        HAL_GPIO_WritePin(STEERING_ENGINE_PORT, STEERING_ENGINE_2_PIN, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(STEERING_ENGINE_PORT, STEERING_ENGINE_2_PIN, GPIO_PIN_RESET);

    if (g_pwm_pulse >= 2000)
        g_pwm_pulse = 0;
}

void steering_engine_test()
{
    printf("%d\n", g_pwm_pulse);
}
