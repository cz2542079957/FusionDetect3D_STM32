#include "motor.h"

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim8;

/*private*/

// 误差校正
int16_t motor_pulse_bias[MOTOR_NUMS] = {0, 0, 0, 0};

// 初始化gpio引脚
void motor_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 设置TIM1对应的GPIO
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_TIM1_PARTIAL();

    // 设置TIM8对应的GPIO
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

// 去除电机死区
int16_t motor_ignore_dead_zone(int16_t pulse)
{
    // 去除死区
    if (pulse > 0)
        pulse += MOTOR_IGNORE_PULSE;
    if (pulse < 0)
        pulse -= MOTOR_IGNORE_PULSE;
    // 限制范围
    return ABS_IN_RANGE(pulse, MOTOR_PULSE_MAX);
}

/*public*/
void motor_init()
{
    motor_gpio_init();

    HAL_StatusTypeDef status;

    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_TIM8_CLK_ENABLE();
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 0;                          // 设置预分频器值
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;       // 或者根据需求选择其他计数模式
    htim1.Init.Period = 3600 - 1;                      // 设置自动重载值（决定PWM周期）
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 设置时钟分割因子
    status = HAL_TIM_Base_Init(&htim1);
    htim8.Instance = TIM8;
    htim8.Init.Prescaler = 0;                          // 设置预分频器值
    htim8.Init.CounterMode = TIM_COUNTERMODE_UP;       // 或者根据需求选择其他计数模式
    htim8.Init.Period = 3600 - 1;                      // 设置自动重载值（决定PWM周期）
    htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 设置时钟分割因子
    status |= HAL_TIM_Base_Init(&htim8);
    if (status != HAL_OK)
    {
        // printf("error1\n");
    }

    // 配置每个通道的PWM模式
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;         // 或者选择合适的PWM模式，如PWM2/PWMn等
    sConfigOC.Pulse = 0;                        // 设置初始占空比
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH; // 设置输出极性，高有效或低有效
    status = HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
    status |= HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2);
    status |= HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3);
    status |= HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4);
    status |= HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1);
    status |= HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_2);
    status |= HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_3);
    status |= HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_4);
    if (status != HAL_OK)
    {
        // printf("error1");
    }

    status = HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    status |= HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
    status |= HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
    status |= HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

    status |= HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
    status |= HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
    status |= HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
    status |= HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
    if (status != HAL_OK)
    {
        // printf("error2");
    }
}

void motor_stop()
{
    motor_set_all_pwm(0);
}

void motor_set_pwm(Motor_ID id, int16_t pulse)
{
    // motor_pulse_bias是对pulse绝对值的修正偏置，所以要与pulse的绝对值相加，得到修正后的pulse绝对值
    if (pulse >= 0)
        pulse = motor_ignore_dead_zone(pulse + motor_pulse_bias[id]);
    else
        pulse = -motor_ignore_dead_zone(-pulse + motor_pulse_bias[id]);
    switch (id)
    {
    case MOTOR_ID_M1:
    {
        if (pulse >= 0)
        {
            PWM_M1_A = pulse;
            PWM_M1_B = 0;
        }
        else
        {
            PWM_M1_A = 0;
            PWM_M1_B = -pulse;
        }
        break;
    }
    case MOTOR_ID_M2:
    {
        pulse = -pulse;
        if (pulse >= 0)
        {
            PWM_M2_A = pulse;
            PWM_M2_B = 0;
        }
        else
        {
            PWM_M2_A = 0;
            PWM_M2_B = -pulse;
        }
        break;
    }
    case MOTOR_ID_M3:
    {
        if (pulse >= 0)
        {
            PWM_M3_A = pulse;
            PWM_M3_B = 0;
        }
        else
        {
            PWM_M3_A = 0;
            PWM_M3_B = -pulse;
        }
        break;
    }
    case MOTOR_ID_M4:
    {
        pulse = -pulse;
        if (pulse >= 0)
        {
            PWM_M4_A = pulse;
            PWM_M4_B = 0;
        }
        else
        {
            PWM_M4_A = 0;
            PWM_M4_B = -pulse;
        }
        break;
    }
    default:
        break;
    }
}

void motor_set_all_pwm(int16_t pulse)
{
    // pulse1-pulse4与pulse一样，带有正负表示方向
    int16_t pulse1 = 0;
    int16_t pulse2 = 0;
    int16_t pulse3 = 0;
    int16_t pulse4 = 0;
    if (pulse > 0)
    {
        pulse1 = motor_ignore_dead_zone(pulse + motor_pulse_bias[0]);
        pulse2 = motor_ignore_dead_zone(pulse + motor_pulse_bias[1]);
        pulse3 = motor_ignore_dead_zone(pulse + motor_pulse_bias[2]);
        pulse4 = motor_ignore_dead_zone(pulse + motor_pulse_bias[3]);
    }
    else
    {
        pulse1 = -motor_ignore_dead_zone(-pulse + motor_pulse_bias[0]);
        pulse2 = -motor_ignore_dead_zone(-pulse + motor_pulse_bias[1]);
        pulse3 = -motor_ignore_dead_zone(-pulse + motor_pulse_bias[2]);
        pulse4 = -motor_ignore_dead_zone(-pulse + motor_pulse_bias[3]);
    }
    // printf("debug: %d\n", speed);
    if (pulse >= 0)
    {
        PWM_M1_A = pulse1;
        PWM_M1_B = 0;
        PWM_M2_A = 0;
        PWM_M2_B = pulse2;
        PWM_M3_A = pulse3;
        PWM_M3_B = 0;
        PWM_M4_A = 0;
        PWM_M4_B = pulse4;
    }
    else
    {
        pulse = motor_ignore_dead_zone(pulse);
        PWM_M1_A = 0;
        PWM_M1_B = -pulse1;
        PWM_M2_A = -pulse2;
        PWM_M2_B = 0;
        PWM_M3_A = 0;
        PWM_M3_B = -pulse3;
        PWM_M4_A = -pulse4;
        PWM_M4_B = 0;
    }
}

float motor_pulse_bias_calculation(int16_t encoder_count_delta)
{
    // encoder_count_delta 是正值
    if (encoder_count_delta <= 1)
        return 1;
    else if (encoder_count_delta > 1)
        // return powf(encoder_count_delta, MOTOR_PULSE_BIAS_CALCULATION_PARAMETER_1) * MOTOR_PULSE_BIAS_CALCULATION_PARAMETER_2;
        return encoder_count_delta * encoder_count_delta * MOTOR_PULSE_BIAS_CALCULATION_PARAMETER_2;
    return 0;
}

void motor_get_all_bias(int16_t *bias)
{
    bias[0] = motor_pulse_bias[0];
    bias[1] = motor_pulse_bias[1];
    bias[2] = motor_pulse_bias[2];
    bias[3] = motor_pulse_bias[3];
}

void motor_set_all_bias(int16_t *bias)
{
    motor_pulse_bias[0] = bias[0];
    motor_pulse_bias[1] = bias[1];
    motor_pulse_bias[2] = bias[2];
    motor_pulse_bias[3] = bias[3];
}
