#include "servo_motor.h"

TIM_HandleTypeDef htim6;

uint16_t pwm_pulse = 0;                                               // 当前脉冲
float target_angle[SERVO_MOTOR_NUMS] = {0};                           // 目标角度
float current_angle[SERVO_MOTOR_NUMS] = {0};                          // 当前角度
float target_pwm_width[SERVO_MOTOR_NUMS] = {50, 50};                  // 目标占空比
float pwm_width[SERVO_MOTOR_NUMS] = {50, 50};                         // 占空比
float angle_pwm_scale_factor[SERVO_MOTOR_NUMS] = {7.40, 11.11};       // 角度占空比映射系数
float pwm_acceleration[SERVO_MOTOR_NUMS] = {7.40 / 200, 11.11 / 200}; // pwm变化加速度

void TIM6_IRQHandler(void)
{
    __HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);
    pwm_pulse++;
    // printf("%u\n", g_pwm_pulse);
    if (pwm_pulse <= pwm_width[SERVO_MOTOR_1])
        HAL_GPIO_WritePin(SERVO_MOTOR_PORT, SERVO_MOTOR_1_PIN, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(SERVO_MOTOR_PORT, SERVO_MOTOR_1_PIN, GPIO_PIN_RESET);

    if (pwm_pulse <= pwm_width[SERVO_MOTOR_2])
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

void servo_motor_set_angle(SERVO_MOTOR_ID id, float angle)
{
    target_angle[id] = angle;
    target_pwm_width[id] = (angle * angle_pwm_scale_factor[id] + 500) / 10;
    // printf("%u\n", g_angle_num[id]);
}

void servo_motor_set_all_angle(float angle)
{
    servo_motor_set_angle(SERVO_MOTOR_1, angle);
    servo_motor_set_angle(SERVO_MOTOR_2, angle);
}

void servo_motor_on_time(uint16_t interval)
{
    float delta1 = interval * pwm_acceleration[SERVO_MOTOR_1],
          delta2 = interval * pwm_acceleration[SERVO_MOTOR_2];
    if (ABS(pwm_width[SERVO_MOTOR_1] - target_pwm_width[SERVO_MOTOR_1]) < delta1)
        pwm_width[SERVO_MOTOR_1] = target_pwm_width[SERVO_MOTOR_1];
    if (pwm_width[SERVO_MOTOR_1] < target_pwm_width[SERVO_MOTOR_1])
        pwm_width[SERVO_MOTOR_1] += delta1;
    else if (pwm_width[SERVO_MOTOR_1] > target_pwm_width[SERVO_MOTOR_1])
        pwm_width[SERVO_MOTOR_1] -= delta1;

    if (ABS(pwm_width[SERVO_MOTOR_2] - target_pwm_width[SERVO_MOTOR_2]) < delta2)
        pwm_width[SERVO_MOTOR_2] = target_pwm_width[SERVO_MOTOR_2];
    if (pwm_width[SERVO_MOTOR_2] < target_pwm_width[SERVO_MOTOR_2])
        pwm_width[SERVO_MOTOR_2] += delta2;
    else if (pwm_width[SERVO_MOTOR_2] > target_pwm_width[SERVO_MOTOR_2])
        pwm_width[SERVO_MOTOR_2] -= delta2;

    current_angle[SERVO_MOTOR_1] = (pwm_width[SERVO_MOTOR_1] * 10 - 500) / angle_pwm_scale_factor[SERVO_MOTOR_1];
    current_angle[SERVO_MOTOR_2] = (pwm_width[SERVO_MOTOR_2] * 10 - 500) / angle_pwm_scale_factor[SERVO_MOTOR_2];
    // printf("current_angle[SERVO_MOTOR_1] = %f\n", current_angle[SERVO_MOTOR_1]);
}
