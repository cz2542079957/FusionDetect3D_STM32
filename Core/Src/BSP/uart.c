#include "uart.h"

UART_HandleTypeDef huart1;

#define UART_BUFFER_SIZE 1024 * 4 + 2
struct Uart_Buffer {
  uint8_t data[UART_BUFFER_SIZE];
  uint16_t size;
  uint16_t head;
  uint16_t tail;
} uart_buffer;
uint8_t temp_data = 0;

int8_t uart_is_buffer_empty() { return uart_buffer.size == 0; }

int8_t uart_is_buffer_full() {
  return uart_buffer.size == (UART_BUFFER_SIZE - 1);
}

void uart_init() {
  huart1.Instance = USART1;
  huart1.Init.BaudRate = BAUDRATE;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  // __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE); // 开启接收数据可用中断
  // HAL_NVIC_EnableIRQ(USART1_IRQn);             // 开启USART1中断
  if (HAL_UART_Init(&huart1) != HAL_OK) {
    // 错误处理
  }

  uart_buffer.size = 0;
  uart_buffer.head = 0;
  uart_buffer.tail = 0;
  HAL_UART_Receive_IT(&huart1, &temp_data, 1);
}

uint16_t uart_get_avaliable() { return uart_buffer.size; }

void uart_recv(uint8_t data) {
  //加入到缓存中
  if (!uart_is_buffer_full()) {
    uart_buffer.data[uart_buffer.tail] = data;
    uart_buffer.tail = (uart_buffer.tail + 1) % UART_BUFFER_SIZE;
    uart_buffer.size++;
  }
}

void uart_send(uint8_t *data, uint16_t length) {
  HAL_UART_Transmit(&huart1, data, length, 10000);
}

void uart_data_handler() {
  uint8_t senddata[3] = {0xff, (uint8_t)(uart_buffer.size >> 8),
                         (uint8_t)(uart_buffer.size & 0xff)};
  uart_send(senddata, 3);
}

void uart_on_time(uint16_t interval) {
  // __HAL_UART_CLEAR_OREFLAG(&huart1);
  // uint8_t data;
  // while (HAL_UART_Receive(&huart1, data, 1, 10000) == HAL_OK) {
  //   uart_recv(data);
  // }
  uart_data_handler();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == &huart1) {
    uart_recv(temp_data);
    HAL_UART_Receive_IT(&huart1, &temp_data, 1);
  }
}