#include "service_servo.h"

float current_angle[SERVO_MOTOR_NUMS] = {0, 0};
float angle_bias[SERVO_MOTOR_NUMS] = {36.0, -8.0};
float angle_pwm_scale_factor[SERVO_MOTOR_NUMS] = {7.35, 11.11};       // 角度占空比映射系数
float pwm_acceleration[SERVO_MOTOR_NUMS] = {7.40 / 200, 11.11 / 200}; // pwm变化加速度

struct Servo_Motor_States servo_motor1_states[MAX_SERVO_MOTOR_STATES]; // 舵机1状态
struct Servo_Motor_States servo_motor2_states[MAX_SERVO_MOTOR_STATES]; // 舵机2状态
uint8_t servo_motor1_states_head = 0, servo_motor2_states_head = 0;
uint8_t servo_motor1_states_rear = -1, servo_motor2_states_rear = -1;
uint8_t servo_motor1_states_size = 0, servo_motor2_states_size = 0;

bool auto_scan_mode = false;     // 自动扫描模式
float scan_range_extend = 12.0f; // 扫描范围扩展

void service_servo_push_state(SERVO_MOTOR_ID id, float angle, float speed)
{
    angle += angle_bias[id];
    if (id == SERVO_MOTOR_1)
    {
        if (servo_motor1_states_size >= MAX_SERVO_MOTOR_STATES)
            return;
        if (angle > MAX_SERVO_MOTOR1_ANGLE)
            angle = MAX_SERVO_MOTOR1_ANGLE;
        if (angle < MIN_SERVO_MOTOR1_ANGLE)
            angle = MIN_SERVO_MOTOR1_ANGLE;
        if (speed > MAX_SERVO_MOTOR1_SPEED)
            speed = MAX_SERVO_MOTOR1_SPEED;
        servo_motor1_states_size++;
        servo_motor1_states_rear = (servo_motor1_states_rear + 1) % MAX_SERVO_MOTOR_STATES;
        servo_motor1_states[servo_motor1_states_rear].pwm_width = (angle * angle_pwm_scale_factor[id] + 500) / 10;
        servo_motor1_states[servo_motor1_states_rear].speed = speed;
    }
    else
    {
        if (servo_motor2_states_size >= MAX_SERVO_MOTOR_STATES)
            return;
        if (angle > MAX_SERVO_MOTOR2_ANGLE)
            angle = MAX_SERVO_MOTOR2_ANGLE;
        if (angle < MIN_SERVO_MOTOR2_ANGLE)
            angle = MIN_SERVO_MOTOR2_ANGLE;
        if (speed > MAX_SERVO_MOTOR2_SPEED)
            speed = MAX_SERVO_MOTOR2_SPEED;
        servo_motor2_states_size++;
        servo_motor2_states_rear = (servo_motor2_states_rear + 1) % MAX_SERVO_MOTOR_STATES;
        servo_motor2_states[servo_motor2_states_rear].pwm_width = (angle * angle_pwm_scale_factor[id] + 500) / 10;
        servo_motor2_states[servo_motor2_states_rear].speed = speed;
    }
}

void service_servo_pop_state(SERVO_MOTOR_ID id)
{
    if (id == SERVO_MOTOR_1)
    {
        if (servo_motor1_states_size == 0)
            return;
        servo_motor1_states_size--;
        servo_motor1_states_head = (servo_motor1_states_head + 1) % MAX_SERVO_MOTOR_STATES;
    }
    else
    {
        if (servo_motor2_states_size == 0)
            return;
        servo_motor2_states_size--;
        servo_motor2_states_head = (servo_motor2_states_head + 1) % MAX_SERVO_MOTOR_STATES;
    }
}

return_code_t service_servo_init()
{
    return_code_t ret = RETURN_OK;
    ret |= servo_motor_init();

    // 自检程序
    service_servo_startup_check();

    return ret;
}

void service_servo_set_angle(SERVO_MOTOR_ID id, float angle)
{
    service_servo_push_state(id, angle, 1.0f);
}

void service_servo_set_angle_speed(SERVO_MOTOR_ID id, float angle, float speed)
{
    service_servo_push_state(id, angle, speed);
}

void service_servo_set_all_angle(float angle)
{
    service_servo_set_angle(SERVO_MOTOR_1, angle);
    service_servo_set_angle(SERVO_MOTOR_2, angle);
}

float *service_servo_get_all_angle()
{
    return current_angle;
}

void service_servo_set_auto_scan(bool val)
{
    if (auto_scan_mode ^ val)
    {
        while (servo_motor1_states_size > 0)
            service_servo_pop_state(SERVO_MOTOR_1);
    }
    auto_scan_mode = val;
}

void service_servo_startup_check()
{
    // service_servo_push_state(SERVO_MOTOR_1, 270, 5.0f);
    // service_servo_push_state(SERVO_MOTOR_2, 180, 4.0f);

    // service_servo_push_state(SERVO_MOTOR_1, 0, 4.0f);
    // service_servo_push_state(SERVO_MOTOR_2, 0, 3.0f);

    service_servo_push_state(SERVO_MOTOR_1, 90, 1.0f);
    service_servo_push_state(SERVO_MOTOR_2, 88, 1.0f);

    // service_servo_push_state(SERVO_MOTOR_1, 180, 2.0f);
}

void service_servo_on_time(uint16_t interval)
{
    // 打印相关数据
    // printf("servo_motor1_states_size: %d\n", servo_motor1_states_size);

    // 运动到指定角度
    if (servo_motor1_states_size > 0)
    {
        float delta1 = interval * pwm_acceleration[SERVO_MOTOR_1] * servo_motor1_states[servo_motor1_states_head].speed;
        if (ABS(servo_motor1_states[servo_motor1_states_head].pwm_width - servo_motor_pwm_width[SERVO_MOTOR_1]) < delta1)
        {
            servo_motor_pwm_width[SERVO_MOTOR_1] = servo_motor1_states[servo_motor1_states_head].pwm_width;
            service_servo_pop_state(SERVO_MOTOR_1);
        }
        else
        {
            if (servo_motor_pwm_width[SERVO_MOTOR_1] < servo_motor1_states[servo_motor1_states_head].pwm_width)
                servo_motor_pwm_width[SERVO_MOTOR_1] += delta1;
            else if (servo_motor_pwm_width[SERVO_MOTOR_1] > servo_motor1_states[servo_motor1_states_head].pwm_width)
                servo_motor_pwm_width[SERVO_MOTOR_1] -= delta1;
        }
    }
    if (servo_motor2_states_size > 0)
    {
        float delta2 = interval * pwm_acceleration[SERVO_MOTOR_2] * servo_motor2_states[servo_motor2_states_head].speed;
        if (ABS(servo_motor2_states[servo_motor2_states_head].pwm_width - servo_motor_pwm_width[SERVO_MOTOR_2]) < delta2)
        {
            servo_motor_pwm_width[SERVO_MOTOR_2] = servo_motor2_states[servo_motor2_states_head].pwm_width;
            service_servo_pop_state(SERVO_MOTOR_2);
        }
        else
        {
            if (servo_motor_pwm_width[SERVO_MOTOR_2] < servo_motor2_states[servo_motor2_states_head].pwm_width)
                servo_motor_pwm_width[SERVO_MOTOR_2] += delta2;
            else if (servo_motor_pwm_width[SERVO_MOTOR_2] > servo_motor2_states[servo_motor2_states_head].pwm_width)
                servo_motor_pwm_width[SERVO_MOTOR_2] -= delta2;
        }
    }

    // 更新当前角度 放弃使用该数据
    // current_angle[SERVO_MOTOR_1] = (servo_motor_pwm_width[SERVO_MOTOR_1] * 10 - 500) / angle_pwm_scale_factor[SERVO_MOTOR_1] - angle_bias[SERVO_MOTOR_1];
    // current_angle[SERVO_MOTOR_2] = (servo_motor_pwm_width[SERVO_MOTOR_2] * 10 - 500) / angle_pwm_scale_factor[SERVO_MOTOR_2] - angle_bias[SERVO_MOTOR_2];

    // 自动扫描模式
    if (auto_scan_mode && servo_motor1_states_size == 0)
    {
        service_servo_push_state(SERVO_MOTOR_1, 0, 1.0f);
        service_servo_push_state(SERVO_MOTOR_1, 180 + scan_range_extend, 0.38f);
        service_servo_push_state(SERVO_MOTOR_1, scan_range_extend, 3.0f);
        service_servo_push_state(SERVO_MOTOR_1, 0, 1.0f);
        service_servo_push_state(SERVO_MOTOR_1, 180 + scan_range_extend, 0.42f);
        service_servo_push_state(SERVO_MOTOR_1, scan_range_extend, 3.0f);
    }
    // printf("current: %f %f\n", current_angle[SERVO_MOTOR_1], current_angle[SERVO_MOTOR_2]);
}
