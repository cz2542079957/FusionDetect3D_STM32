#pragma once
#include "uart.h"

#include "beep.h"
#include "key1.h"
#include "led.h"
#include "icm20948_api.h"

#include "service_motion.h"

// 帧头
#define FRAME_HEAD1 0xFF
#define FRAME_HEAD2 0xCC
// 帧头长度(包含两个帧头和长度值，共3字节)
#define FRAME_HEAD_LENGTH 3
// 最短帧长
#define MIN_FRAME_LENGTH 4
// 开启校验和
#define FRAME_ENABLE_CHECK_SUM 1

/*串口功能头标志*/
typedef enum
{
    FRAME_FUNC_ENABLE_AUTO_SEND = 0x01, // 自动发送
    FRAME_FUNC_BEEP_ONCE = 0x11,        // 蜂鸣器响一声
    FRAME_FUNC_LED_KEEP_ON = 0x21,      // LED 常亮
    FRAME_FUNC_LED_KEEP_OFF = 0x22,     // LED 熄灭
    FRAME_FUNC_LED_FLASH = 0x23,        // LED 闪烁
    FRAME_FUNC_MOTION = 0x61,           // 小车运动控制
    FRAME_FUNC_ENCODER = 0X71,          // 编码器

} Frame_Func_Flag;

// 硬件初始化
void bsp_init();

// 主周期轮询
void main_on_time(uint16_t interval);
// 次周期轮询
void sub_on_time(uint16_t interval);
