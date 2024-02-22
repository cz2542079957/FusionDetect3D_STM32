#include "service_motion.h"

// 当前运动状态
int8_t motion_state = MOTION_STOP;
// 当前电机脉冲
int16_t current_motor_pulse[MOTOR_NUMS] = {0, 0, 0, 0};
// 目标脉冲
int16_t target_motor_pulse[MOTOR_NUMS] = {0, 0, 0, 0};

// 速度-脉冲映射
int16_t motion_speed_to_pulse(int16_t speed)
{
    return speed * 16;
}

void motion_stop()
{
    for (int i = 0; i < MOTOR_NUMS; i++)
        target_motor_pulse[i] = 0;
    motion_state = MOTION_STOP;
}

void motion_go_forward(int16_t speed)
{
    // printf("motion_state: %d\n", motion_state);
    int16_t pulse = motion_speed_to_pulse(speed);
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
    int16_t pulse = motion_speed_to_pulse(-speed);
    for (int i = 0; i < MOTOR_NUMS; i++)
        target_motor_pulse[i] = pulse;
    if (motion_state != MOTION_BACKWARD)
    {
        for (int i = 0; i < MOTOR_NUMS; i++)
            current_motor_pulse[i] = 0;
        motion_state = MOTION_BACKWARD;
    }
}

void motion_go_right(int16_t speed)
{
    int16_t pulse = motion_speed_to_pulse(speed);
    target_motor_pulse[0] = pulse;
    target_motor_pulse[1] = -pulse;
    target_motor_pulse[2] = -pulse;
    target_motor_pulse[3] = pulse;
    if (motion_state != MOTION_RIGHT)
    {
        for (int i = 0; i < MOTOR_NUMS; i++)
            current_motor_pulse[i] = 0;
        motion_state = MOTION_RIGHT;
    }
}

void motion_go_left(int16_t speed)
{
    int16_t pulse = motion_speed_to_pulse(speed);
    target_motor_pulse[0] = -pulse;
    target_motor_pulse[1] = pulse;
    target_motor_pulse[2] = pulse;
    target_motor_pulse[3] = -pulse;
    if (motion_state != MOTION_LEFT)
    {
        for (int i = 0; i < MOTOR_NUMS; i++)
            current_motor_pulse[i] = 0;
        motion_state = MOTION_LEFT;
    }
}

void motion_go_front_right(int16_t speed)
{
    int16_t pulse = motion_speed_to_pulse(speed);
    target_motor_pulse[0] = pulse;
    target_motor_pulse[1] = 0;
    target_motor_pulse[2] = 0;
    target_motor_pulse[3] = pulse;
    if (motion_state != MOTION_FRONT_RIGHT)
    {
        for (int i = 0; i < MOTOR_NUMS; i++)
            current_motor_pulse[i] = 0;
        motion_state = MOTION_FRONT_RIGHT;
    }
}

void motion_go_front_left(int16_t speed)
{
    int16_t pulse = motion_speed_to_pulse(speed);
    target_motor_pulse[0] = 0;
    target_motor_pulse[1] = pulse;
    target_motor_pulse[2] = pulse;
    target_motor_pulse[3] = 0;
    if (motion_state != MOTION_FRONT_LEFT)
    {
        for (int i = 0; i < MOTOR_NUMS; i++)
            current_motor_pulse[i] = 0;
        motion_state = MOTION_FRONT_LEFT;
    }
}

void motion_go_back_right(int16_t speed)
{
    int16_t pulse = motion_speed_to_pulse(speed);
    target_motor_pulse[0] = 0;
    target_motor_pulse[1] = -pulse;
    target_motor_pulse[2] = -pulse;
    target_motor_pulse[3] = 0;
    if (motion_state != MOTION_FRONT_LEFT)
    {
        for (int i = 0; i < MOTOR_NUMS; i++)
            current_motor_pulse[i] = 0;
        motion_state = MOTION_FRONT_LEFT;
    }
}

void motion_go_back_left(int16_t speed)
{
    int16_t pulse = motion_speed_to_pulse(speed);
    target_motor_pulse[0] = -pulse;
    target_motor_pulse[1] = 0;
    target_motor_pulse[2] = 0;
    target_motor_pulse[3] = -pulse;
    if (motion_state != MOTION_BACK_LEFT)
    {
        for (int i = 0; i < MOTOR_NUMS; i++)
            current_motor_pulse[i] = 0;
        motion_state = MOTION_BACK_LEFT;
    }
}

void motion_turn_right(int16_t speed)
{
    int16_t pulse = motion_speed_to_pulse(speed);
    target_motor_pulse[0] = -pulse;
    target_motor_pulse[1] = pulse;
    target_motor_pulse[2] = -pulse;
    target_motor_pulse[3] = pulse;
    if (motion_state != MOTION_TURN_RIGHT)
    {
        for (int i = 0; i < MOTOR_NUMS; i++)
            current_motor_pulse[i] = 0;
        motion_state = MOTION_TURN_RIGHT;
    }
}

void motion_turn_left(int16_t speed)
{
    int16_t pulse = motion_speed_to_pulse(speed);
    target_motor_pulse[0] = pulse;
    target_motor_pulse[1] = -pulse;
    target_motor_pulse[2] = pulse;
    target_motor_pulse[3] = -pulse;
    if (motion_state != MOTION_TURN_LEFT)
    {
        for (int i = 0; i < MOTOR_NUMS; i++)
            current_motor_pulse[i] = 0;
        motion_state = MOTION_TURN_LEFT;
    }
}

void motion_turn_front_right(int16_t speed)
{
    int16_t pulse = motion_speed_to_pulse(speed);
    target_motor_pulse[0] = -pulse;
    target_motor_pulse[1] = pulse;
    target_motor_pulse[2] = 0;
    target_motor_pulse[3] = 0;
    if (motion_state != MOTION_TURN_FRONT_RIGHT)
    {
        for (int i = 0; i < MOTOR_NUMS; i++)
            current_motor_pulse[i] = 0;
        motion_state = MOTION_TURN_FRONT_RIGHT;
    }
}

void motion_turn_front_left(int16_t speed)
{
    int16_t pulse = motion_speed_to_pulse(speed);
    target_motor_pulse[0] = pulse;
    target_motor_pulse[1] = -pulse;
    target_motor_pulse[2] = 0;
    target_motor_pulse[3] = 0;
    if (motion_state != MOTION_TURN_FRONT_LEFT)
    {
        for (int i = 0; i < MOTOR_NUMS; i++)
            current_motor_pulse[i] = 0;
        motion_state = MOTION_TURN_FRONT_LEFT;
    }
}

void motion_turn_back_right(int16_t speed)
{
    int16_t pulse = motion_speed_to_pulse(speed);
    target_motor_pulse[0] = 0;
    target_motor_pulse[1] = 0;
    target_motor_pulse[2] = -pulse;
    target_motor_pulse[3] = pulse;
    if (motion_state != MOTION_TURN_BACK_RIGHT)
    {
        for (int i = 0; i < MOTOR_NUMS; i++)
            current_motor_pulse[i] = 0;
        motion_state = MOTION_TURN_BACK_RIGHT;
    }
}

void motion_turn_back_left(int16_t speed)
{
    int16_t pulse = motion_speed_to_pulse(speed);
    target_motor_pulse[0] = 0;
    target_motor_pulse[1] = 0;
    target_motor_pulse[2] = pulse;
    target_motor_pulse[3] = -pulse;
    if (motion_state != MOTION_TURN_BACK_LEFT)
    {
        for (int i = 0; i < MOTOR_NUMS; i++)
            current_motor_pulse[i] = 0;
        motion_state = MOTION_TURN_BACK_LEFT;
    }
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
        int16_t speed_delta = MOTOR_SPEED_INCREMENT_COE * interval;
        int16_t new_pulse = target_motor_pulse[i] > current_motor_pulse[i]
                                ? (current_motor_pulse[i] + speed_delta)
                                : (current_motor_pulse[i] - speed_delta);
        // 如果距离目标较小，则直接赋值相等
        if (ABS(new_pulse - target_motor_pulse[i]) <= speed_delta)
            new_pulse = target_motor_pulse[i];
        current_motor_pulse[i] = new_pulse;
        motor_set_pwm(i, new_pulse);
    }
}

void motion_parse_command(Motion_State state, uint16_t speed)
{
    switch (state)
    {
    default:
    case MOTION_STOP:
        motion_stop();
        break;
    case MOTION_FORWARD:
        motion_go_forward(speed);
        break;
    case MOTION_BACKWARD:
        motion_go_backward(speed);
        break;
    case MOTION_LEFT:
        motion_go_left(speed);
        break;
    case MOTION_RIGHT:
        motion_go_right(speed);
        break;
    case MOTION_FRONT_RIGHT:
        motion_turn_front_right(speed);
        break;
    case MOTION_FRONT_LEFT:
        motion_turn_front_left(speed);
        break;
    case MOTION_BACK_RIGHT:
        motion_turn_back_right(speed);
        break;
    case MOTION_BACK_LEFT:
        motion_turn_back_left(speed);
        break;
    case MOTION_TURN_LEFT:
        motion_turn_left(speed);
        break;
    case MOTION_TURN_RIGHT:
        motion_turn_right(speed);
        break;
    case MOTION_TURN_FRONT_RIGHT:
        motion_turn_front_right(speed);
        break;
    case MOTION_TURN_FRONT_LEFT:
        motion_turn_front_left(speed);
        break;
    case MOTION_TURN_BACK_RIGHT:
        motion_turn_back_right(speed);
        break;
    case MOTION_TURN_BACK_LEFT:
        motion_turn_back_left(speed);
        break;
    }
}
