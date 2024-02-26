#include "service_protocol.h"

/* private */
int enable_check_sum = FRAME_ENABLE_CHECK_SUM; // 是否开启校验和

// 用于解析串口命令
void uart_data_paser()
{
    uint8_t index = uart_buffer.head;
    // uint16_t size = uart_buffer.size;
    // 去除损坏帧
    while (!(uart_buffer.data[index] == FRAME_HEAD1 &&
             uart_buffer.data[uart_get_index(index + 1)] == FRAME_HEAD2) &&
           uart_buffer.size > 0)
    {
        index = uart_get_index(index + 1);
        uart_buffer.size--;
    }
    uart_buffer.head = index;
    // 开始处理数据
    while (uart_buffer.size >= MIN_FRAME_LENGTH)
    {
        // 包头比对
        if (uart_buffer.data[index] == FRAME_HEAD1 &&
            uart_buffer.data[uart_get_index(index + 1)] == FRAME_HEAD2)
        {
            // 帧长度
            uint8_t frame_length = uart_buffer.data[uart_get_index(index + 2)] + FRAME_HEAD_LENGTH;
            // 剩余数据不完整
            if (uart_buffer.size < frame_length)
                return;
            // 获取功能字
            uint8_t function = uart_buffer.data[uart_get_index(index + 3)];
            // 校验和
            if (enable_check_sum)
            {
            }

            printf("frame_length: %d function: %d\n", frame_length, function);

            switch (function)
            {
            case FRAME_FUNC_ENABLE_AUTO_SEND:
                break;
            case FRAME_FUNC_BEEP_ONCE:
                beep_once(20);
                break;
            case FRAME_FUNC_LED_KEEP_ON:
                led_keep_on();
                break;
            case FRAME_FUNC_LED_KEEP_OFF:
                led_keep_off();
                break;
            case FRAME_FUNC_LED_FLASH:
                led_flash_with_interval(10, 30);
                break;
            case FRAME_FUNC_MOTION:
                uint8_t state = uart_buffer.data[uart_get_index(index + 4)]; // 运动状态
                uint8_t speed = uart_buffer.data[uart_get_index(index + 5)]; // 速度
                motion_parse_command(state, speed);
                break;
            }
            uart_buffer.head = uart_get_index(uart_buffer.head + frame_length);
            uart_buffer.size -= frame_length;
        }

        // printf("uart_buffer.size: %u\n", uart_buffer.size);
        // printf("uart_buffer.head: %u\n", uart_buffer.head);
        // printf("uart_buffer.tail: %u\n", uart_buffer.tail);
    }
}

// 用于打包发送各种数据
void uart_data_publisher()
{
    // printf("uart_buffer.size: %d\n", uart_buffer.size);
}

/* public */
void service_protocol_on_time(uint16_t interval)
{
    uart_data_paser();
    uart_data_publisher();
}