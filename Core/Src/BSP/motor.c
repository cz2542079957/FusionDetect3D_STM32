#include "motor.h"
#include "stdio.h"

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim8;

/*private*/

// 当前运动状态
int8_t motion_state = MOTION_STOP;
// 当前电机脉冲
int16_t current_motor_pulse[MOTOR_NUMS] = {0, 0, 0, 0};
// 目标脉冲
int16_t target_motor_pulse[MOTOR_NUMS] = {0, 0, 0, 0};

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
    if (pulse > MOTOR_SPEED_MAX)
        pulse = MOTOR_SPEED_MAX;
    else if (pulse < MOTOR_SPEED_MIN)
        pulse = MOTOR_SPEED_MIN;
    return pulse;
}

// 速度-脉冲映射
int16_t motor_speed_to_pulse(int16_t speed)
{
    return speed * 16;
}

/*public*/
void motor_init()
{
    motor_gpio_init();

    HAL_StatusTypeDef status;

    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_TIM8_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_TIM1_PARTIAL();
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
    status = HAL_TIM_Base_Init(&htim8);
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
    pulse = motor_ignore_dead_zone(pulse);

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
    }
}

void motor_set_all_pwm(int16_t pulse)
{
    pulse = motor_ignore_dead_zone(pulse);
    // printf("debug: %d\n", speed);
    if (pulse >= 0)
    {
        PWM_M1_A = pulse;
        PWM_M1_B = 0;
        PWM_M2_A = 0;
        PWM_M2_B = pulse;
        PWM_M3_A = pulse;
        PWM_M3_B = 0;
        PWM_M4_A = 0;
        PWM_M4_B = pulse;
    }
    else
    {
        PWM_M1_A = 0;
        PWM_M1_B = -pulse;
        PWM_M2_A = -pulse;
        PWM_M2_B = 0;
        PWM_M3_A = 0;
        PWM_M3_B = -pulse;
        PWM_M4_A = -pulse;
        PWM_M4_B = 0;
    }
}

void motor_set_move(Motor_ID id, Motor_Move direction)
{
    switch (id)
    {
    case MOTOR_ID_M1:
    {
        if (direction == MOTOR_FRONT)
        { // 正向
        }
        else if (direction == MOTOR_BACK)
        { // 逆向
        }
        else if (direction == MOTOR_STOP)
        { // 停止
        }
        break;
    }
    case MOTOR_ID_M2:
    {
        if (direction == MOTOR_FRONT)
        { // 正向
        }
        else if (direction == MOTOR_BACK)
        { // 逆向
        }
        else if (direction == MOTOR_STOP)
        { // 停止
        }
        break;
    }
    case MOTOR_ID_M3:
    {
        if (direction == MOTOR_FRONT)
        { // 正向
        }
        else if (direction == MOTOR_BACK)
        { // 逆向
        }
        else if (direction == MOTOR_STOP)
        { // 停止
        }
        break;
    }
    case MOTOR_ID_M4:
    {
        if (direction == MOTOR_FRONT)
        { // 正向
        }
        else if (direction == MOTOR_BACK)
        { // 逆向
        }
        else if (direction == MOTOR_STOP)
        { // 停止
        }
        break;
    }
    }
}

void motion_go_forward(int16_t speed)
{
    // printf("motion_state: %d\n", motion_state);
    int16_t pulse = motor_speed_to_pulse(speed);
    for (int i = 0; i < MOTOR_NUMS; i++)
        target_motor_pulse[i] = pulse;
    if (motion_state != MOTION_FORWARD)
    {
        for (int i = 0; i < MOTOR_NUMS; i++)
            current_motor_pulse[i] = 0;
        motion_state = MOTION_FORWARD;
    }
}

void motion_go_backward(int16_t speed)
{
    int16_t pulse = motor_speed_to_pulse(-speed);
    for (int i = 0; i < MOTOR_NUMS; i++)
        target_motor_pulse[i] = pulse;
    if (motion_state != MOTION_BACKWARD)
    {
        for (int i = 0; i < MOTOR_NUMS; i++)
            current_motor_pulse[i] = 0;
        motion_state = MOTION_BACKWARD;
    }
}

void motion_go_left(int16_t speed) {}

void motion_go_right(int16_t speed) {}

void motion_stop()
{
    for (int i = 0; i < MOTOR_NUMS; i++)
        target_motor_pulse[i] = 0;
    motion_state = MOTION_STOP;
}

void motion_on_time(uint16_t interval)
{
    // printf("motion_state: %d\n", motion_state);

    // printf("current: %d %d %d %d; target: %d %d %d %d\n", current_motor_pulse[0], current_motor_pulse[1],
    //        current_motor_pulse[2], current_motor_pulse[3], target_motor_pulse[0], target_motor_pulse[1],
    //        target_motor_pulse[2], target_motor_pulse[3]);
    for (uint16_t i = 0; i < MOTOR_NUMS; i++)
    {
        if (current_motor_pulse[i] == target_motor_pulse[i])
            continue;
        int16_t new_pulse = target_motor_pulse[i] > current_motor_pulse[i]
                                ? (current_motor_pulse[i] + MOTOR_SPEED_RATE)
                                : (current_motor_pulse[i] - MOTOR_SPEED_RATE);
        // 如果距离目标较小，则直接赋值相等
        int16_t temp = new_pulse > target_motor_pulse[i]
                           ? (new_pulse - target_motor_pulse[i])
                           : (target_motor_pulse[i] - new_pulse);
        if (temp <= MOTOR_SPEED_RATE)
            new_pulse = target_motor_pulse[i];
        current_motor_pulse[i] = new_pulse;
        motor_set_pwm(i, new_pulse);
    }
    return;
    switch (motion_state)
    {
    case MOTION_STOP:
        current_motor_pulse[0] = 0;
        current_motor_pulse[1] = 0;
        current_motor_pulse[2] = 0;
        current_motor_pulse[3] = 0;
        motor_stop();
        break;
    case MOTION_FORWARD:
    case MOTION_BACKWARD:
        if (current_motor_pulse[0] == target_motor_pulse[0])
            break;
        int16_t new_pulse = target_motor_pulse[0] > current_motor_pulse[0]
                                ? (current_motor_pulse[0] + MOTOR_SPEED_RATE)
                                : (current_motor_pulse[0] - MOTOR_SPEED_RATE);
        // 如果距离目标较小，则直接赋值相等
        int16_t temp = new_pulse > target_motor_pulse[0]
                           ? (new_pulse - target_motor_pulse[0])
                           : (target_motor_pulse[0] - new_pulse);
        if (temp <= MOTOR_SPEED_RATE)
            new_pulse = target_motor_pulse[0];
        current_motor_pulse[0] = new_pulse;
        current_motor_pulse[1] = new_pulse;
        current_motor_pulse[2] = new_pulse;
        current_motor_pulse[3] = new_pulse;
        motor_set_all_pwm(new_pulse);
        break;
    case MOTION_LEFT:
        break;
    case MOTION_RIGHT:
        break;
    }
}
