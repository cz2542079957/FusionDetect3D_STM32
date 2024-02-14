#pragma once
#include "beep.h"
#include "key1.h"
#include "led.h"
#include "uart.h"

//帧头
#define FRAME_HEAD1 0xFF
#define FRAME_HEAD2 0xCC
//帧头长度(包含两个帧头和长度值，共3字节)
#define FRAME_HEAD_LENGTH 3
//最短帧长
#define MIN_FRAME_LENGTH 5
//默认是否开启校验和
#define DEFAULT_CHECK_SUM 0

// 硬件初始化
void bsp_init();

// 周期轮询
void on_time(uint16_t interval);