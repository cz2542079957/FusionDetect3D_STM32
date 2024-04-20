#include "uart.h"
#include <string.h>

/* private */
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
uint8_t uart_it_buffer; // 中断接收缓冲区

// 重定向printf
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
    return ch;
}

// 中断处理函数
void USART1_IRQHandler(void)
{
    if ((__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET) &&
        (__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_RXNE) != RESET))
    {
        uint8_t temp = USART1->DR & 0xFF;
        uart_recv(temp);
        // printf("buffer: %u\n", temp);
        // printf("%u\n", temp);
        __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
    }
}

/*
void DMA1_Channel4_IRQHandler(void)
{
    if ((__HAL_DMA_GET_FLAG(&hdma_usart1_tx, DMA_FLAG_TC4)) != RESET)
    {
        if (__HAL_DMA_GET_IT_SOURCE(&hdma_usart1_tx, DMA_IT_TC) != RESET)
        {
            __HAL_DMA_CLEAR_FLAG(&hdma_usart1_tx, DMA_FLAG_TC4);
            __HAL_DMA_DISABLE_IT(&hdma_usart1_tx, DMA_IT_TC);

            // printf("123");

            // 在这里处理接收到的数据，例如：
            // ProcessReceivedData(uart_buffer.data, UART_BUFFER_SIZE);
        }
    }
}
*/

/*
void uart_dma_init()
{
    // USART1 DMA Init
    __HAL_RCC_DMA1_CLK_ENABLE();
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
    }

    __HAL_LINKDMA(&huart1, hdmatx, hdma_usart1_tx);

    // 开启dma中断
    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

    // HAL_UART_Receive_DMA(&huart1, uart_dma_buffer, 5);
}
 */

/* public */

struct Uart_Buffer uart_buffer; // 接收缓冲区

uint8_t uart_is_buffer_empty() { return uart_buffer.size == 0; }

uint8_t uart_is_buffer_full() { return uart_buffer.size >= (UART_BUFFER_SIZE - 1); }

uint16_t uart_get_index(uint16_t index) { return index % UART_BUFFER_SIZE; }

void uart_init()
{
    // 开启USART1时钟和GPIO时钟
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = UART_UART1_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(UART_UART1_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = UART_UART1_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(UART_UART1_PORT, &GPIO_InitStruct);

    // USART1
    huart1.Instance = USART1;
    huart1.Init.BaudRate = BAUDRATE;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        // 错误处理
    }

    // 启动中断
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    HAL_UART_Receive_IT(&huart1, &uart_it_buffer, 1);

    // // 启动dma
    // uart_dma_init();

    // 数据结构初始化
    memset(uart_buffer.data, 0, UART_BUFFER_SIZE);
    uart_buffer.size = 0;
    uart_buffer.head = 0;
    uart_buffer.tail = 0;
}

void uart_recv(uint8_t data)
{
    // 加入到缓存中
    if (!uart_is_buffer_full())
    {
        uart_buffer.data[uart_buffer.tail] = data;
        uart_buffer.tail = uart_get_index(uart_buffer.tail + 1);
        uart_buffer.size++;
    }
}

void uart_send(uint8_t *data, uint16_t length)
{
    HAL_UART_Transmit(&huart1, data, length, 0xffff);
}
