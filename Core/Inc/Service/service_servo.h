#pragma once
#include "servo_motor.h"

struct Servo_Motor_States
{
    float pwm_width;
    float speed;
};
#define MAX_SERVO_MOTOR_STATES 64

#define MAX_SERVO_MOTOR1_SPEED 8.0f
#define MAX_SERVO_MOTOR1_ANGLE 270.0f
#define MIN_SERVO_MOTOR1_ANGLE 0.0f

#define MAX_SERVO_MOTOR2_SPEED 6.0f
#define MAX_SERVO_MOTOR2_ANGLE 170.0f
#define MIN_SERVO_MOTOR2_ANGLE 0.0f

return_code_t service_servo_init();

void service_servo_set_angle(SERVO_MOTOR_ID id, float angle);

void service_servo_set_angle_speed(SERVO_MOTOR_ID id, float angle, float speed);

void service_servo_set_all_angle(float angle);

float* service_servo_get_all_angle();

/**
 * @description: 开机自检
 * @return {*}
 */
void service_servo_startup_check();

/**
 * @description: 周期函数
 * @param {uint16_t} interval
 * @return {*}
 */
void service_servo_on_time(uint16_t interval);