#include "servo_motor.h"

TIM_HandleTypeDef htim6;

uint16_t pwm_pulse = 0;                                   // 当前脉冲
float servo_motor_pwm_width[SERVO_MOTOR_NUMS] = {50, 50}; // 占空比

void TIM6_IRQHandler(void)
{
    __HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);
    pwm_pulse++;
    // printf("%u\n", g_pwm_pulse);
    if (pwm_pulse <= servo_motor_pwm_width[SERVO_MOTOR_1])
        HAL_GPIO_WritePin(SERVO_MOTOR_PORT, SERVO_MOTOR_1_PIN, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(SERVO_MOTOR_PORT, SERVO_MOTOR_1_PIN, GPIO_PIN_RESET);

    if (pwm_pulse <= servo_motor_pwm_width[SERVO_MOTOR_2])
        HAL_GPIO_WritePin(SERVO_MOTOR_PORT, SERVO_MOTOR_2_PIN, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(SERVO_MOTOR_PORT, SERVO_MOTOR_2_PIN, GPIO_PIN_RESET);

    if (pwm_pulse >= 600)
        pwm_pulse = 0;
}

return_code_t servo_motor_gpio_init()
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = SERVO_MOTOR_1_PIN | SERVO_MOTOR_2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SERVO_MOTOR_PORT, &GPIO_InitStruct);
    return RETURN_OK;
}

return_code_t servo_motor_init()
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
    if (servo_motor_gpio_init() != RETURN_OK)
    {
        return RETURN_FAIL;
    }

    return RETURN_OK;
}
