#pragma once
#include "stm32f1xx.h"
#include <stdio.h>

#define UART_UART1_PORT GPIOA
#define UART_UART1_TX_PIN GPIO_PIN_9
#define UART_UART1_RX_PIN GPIO_PIN_10
// #define UART_ENABLE_DMA
#define BAUDRATE 115200 // 波特率

/* 循环数组 */
#define UART_BUFFER_SIZE 1024u
struct Uart_Buffer
{
    uint8_t data[UART_BUFFER_SIZE];
    uint16_t size;
    uint16_t head;
    uint16_t tail;
};
extern struct Uart_Buffer uart_buffer;
// 缓冲是否空
uint8_t uart_is_buffer_empty();
// 缓冲是否满
uint8_t uart_is_buffer_full();
// 获取有效索引
uint16_t uart_get_index(uint16_t index);

// 初始化
void uart_init();
// 接收
void uart_recv(uint8_t data);
// 发送
void uart_send(uint8_t *data, uint16_t length);