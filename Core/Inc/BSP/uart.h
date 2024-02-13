#pragma once
#include "stm32f1xx.h"

// 波特率
#define BAUDRATE 115200

// 初始化
void uart_init();
// 获取可接收的字节数
uint16_t uart_get_avaliable();
// 接收
void uart_recv(uint8_t data);

// 发送
void uart_send(uint8_t *data, uint16_t length);

//轮询函数
void uart_on_time(uint16_t interval);
