#pragma once
#include "stm32f1xx.h"
#include <stdio.h>

// 波特率
#define BAUDRATE 115200

// 循环数组
#define UART_BUFFER_SIZE 4098u
struct Uart_Buffer
{
    uint8_t data[UART_BUFFER_SIZE];
    uint16_t size;
    uint16_t head;
    uint16_t tail;
};
extern struct Uart_Buffer uart_buffer;
uint8_t uart_is_buffer_empty();
uint8_t uart_is_buffer_full();
uint16_t uart_get_index(uint16_t index);

// 初始化
void uart_init();
// 接收
void uart_recv(uint8_t data);

// 发送
void uart_send(uint8_t *data, uint16_t length);

// 轮询函数
void uart_on_time(uint16_t interval);
