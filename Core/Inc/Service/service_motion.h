#pragma once
#include "motor.h"
#include "encoder.h"

// 运动状态
typedef enum
{
    MOTION_STOP,
    MOTION_FORWARD,
    MOTION_BACKWARD,
    MOTION_LEFT,
    MOTION_RIGHT,
    MOTION_FRONT_RIGHT,
    MOTION_FRONT_LEFT,
    MOTION_BACK_RIGHT,
    MOTION_BACK_LEFT,
    MOTION_TURN_LEFT,
    MOTION_TURN_RIGHT,
    MOTION_TURN_FRONT_RIGHT,
    MOTION_TURN_FRONT_LEFT,
    MOTION_TURN_BACK_RIGHT,
    MOTION_TURN_BACK_LEFT,
} Motion_State;

// 初始化
return_code_t motion_init();

// 运动控制(速度值0-100)
void motion_stop();                          // 停止
void motion_go_forward(int16_t speed);       // 前进
void motion_go_backward(int16_t speed);      // 后退
void motion_go_right(int16_t speed);         // 右移
void motion_go_left(int16_t speed);          // 左移
void motion_go_front_right(int16_t speed);   // 右前方
void motion_go_front_left(int16_t speed);    // 左前方
void motion_go_back_right(int16_t speed);    // 右后方
void motion_go_back_left(int16_t speed);     // 左后方
void motion_turn_left(int16_t speed);        // 左转
void motion_turn_right(int16_t speed);       // 右转
void motion_turn_front_right(int16_t speed); // 绕前轮中点右旋
void motion_turn_front_left(int16_t speed);  // 绕前轮中点左旋
void motion_turn_back_right(int16_t speed);  // 绕后轮中点右旋
void motion_turn_back_left(int16_t speed);   // 绕后轮中点左旋

// 轮询函数
void motion_on_time(uint16_t interval);
// 闭环控制函数
void motion_closed_loop_control();
// 控制命令解析函数
void motion_parse_command(Motion_State state, uint16_t speed);

/*
    前进 ffcc03610110
    后退 ffcc03610210
    右移 ffcc03610310
    左移 ffcc03610410
    左前 ffcc03610610
    右后 ffcc03610710
    左后 ffcc03610810
    右前 ffcc03610510
    左转 ffcc03610910
    右转 ffcc03610a10
    绕前轮中点右旋 ffcc03610b10
    绕前轮中点左旋 ffcc03610c10
    绕后轮中点右旋 ffcc03610d10
    绕后轮中点左旋 ffcc03610e10
*/