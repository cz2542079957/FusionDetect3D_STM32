#pragma once
#include "uart.h"

/* 串口帧解析 */
#define FRAME_HEAD1 0xFF // 帧头1
#define FRAME_HEAD2 0xCC // 帧头2
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

// 轮询函数
void service_protocol_on_time(uint16_t interval);