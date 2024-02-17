#include "bsp.h"
// 是否开启校验和
int enable_check_sum = FRAME_ENABLE_CHECK_SUM;

/*private*/

// 各硬件功能融合组成服务
void on_time_service();
// 用于解析串口命令
void uart_data_paser();
// 用于打包发送各种数据
void uart_data_publisher();

/*public*/
void bsp_init()
{
    // 串口通讯初始化
    uart_init();
    // 板载按键1初始化
    key1_init();
    //  led灯初始化
    led_init();
    // 蜂鸣器初始化
    beep_init();
    // motor初始化
    motor_init();
}
void on_time(uint16_t interval)
{
    // key1
    key1_on_time(interval);

    // led
    led_on_time(interval);
    // beep
    beep_on_time(interval);
    // motor - motion
    motion_on_time(interval);

    // 服务
    on_time_service();
}

/*实现 */
void on_time_service()
{
    // 串口数据解析 收发数据
    uart_data_paser();
    uart_data_publisher();

    // 按键key1短按、长按功能
    static uint8_t last_key1_state = KEY1_RELEASE;
    if (key1_get_state() == KEY1_PRESS)
    {
        // 检测为按下
        motion_go_forward(10);
        beep_once(30);
    }
    else if (key1_get_state() == KEY1_LONG_PRESS &&
             last_key1_state != KEY1_LONG_PRESS)
    {
        // 检测长按
        led_flash_with_interval(10, 30);
        led_keep_on();
    }
    last_key1_state = key1_get_state();
}

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

                // case FRAME_FUNC_MOTION_STOP:
                //     motion_stop();
                //     break;
                // case FRAME_FUNC_MOTION_FORWARD:
                //     motion_go_forward(20);
                //     break;
                // case FRAME_FUNC_MOTION_BACKWARD:
                //     motion_go_backward(20);
                //     break;
                // case FRAME_FUNC_MOTION_LEFT:
                //     motion_go_left(20);
                //     break;
                // case FRAME_FUNC_MOTION_RIGHT:
                //     motion_go_right(20);
                //     break;
                // case FRAME_FUNC_MOTION_FRONT_RIGHT:
                //     motion_go_front_right(20);
                //     break;
                // case FRAME_FUNC_MOTION_FRONT_LEFT:
                //     motion_go_front_left(20);
                //     break;
                // case FRAME_FUNC_MOTION_BACK_RIGHT:
                //     motion_go_back_right(20);
                //     break;
                // case FRAME_FUNC_MOTION_BACK_LEFT:
                //     motion_go_back_left(20);
                //     break;
                // case FRAME_FUNC_MOTION_TURN_RIGHT:
                //     motion_turn_right(20);
                //     break;
                // case FRAME_FUNC_MOTION_TURN_LEFT:
                //     motion_turn_left(20);
                //     break;
                // case FRAME_FUNC_MOTION_TURN_FRONT_RIGHT:
                //     motion_turn_front_right(20);
                //     break;
                // case FRAME_FUNC_MOTION_TURN_FRONT_LEFT:
                //     motion_turn_front_left(20);
                //     break;
                // case FRAME_FUNC_MOTION_TURN_BACK_RIGHT:
                //     motion_turn_back_right(20);
                //     break;
                // case FRAME_FUNC_MOTION_TURN_BACK_LEFT:
                //     motion_turn_back_left(20);
                //     break;
            }
            uart_buffer.head = uart_get_index(uart_buffer.head + frame_length);
            uart_buffer.size -= frame_length;
        }

        // printf("uart_buffer.size: %u\n", uart_buffer.size);
        // printf("uart_buffer.head: %u\n", uart_buffer.head);
        // printf("uart_buffer.tail: %u\n", uart_buffer.tail);
    }
}

void uart_data_publisher()
{
    // printf("uart_buffer.size: %d\n", uart_buffer.size);
}
