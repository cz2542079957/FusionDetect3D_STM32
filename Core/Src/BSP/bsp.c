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
    // 编码器初始化
    encoder_init();

    // icm20948初始化

    icm20948_return_code_t ret = ICM20948_RET_OK;
    icm20948_settings_t settings;
//    icm20948_gyro_t gyro_data;
//    icm20948_accel_t accel_data;

    // Init the device function pointers
    ret = icm20948_init(usr_read, usr_write, usr_delay_us);

    // Check if we successfully stored the function poiners provided
    if (ret == ICM20948_RET_OK)
    {
        // Enable the Gyro
        settings.gyro.en = ICM20948_MOD_ENABLED;
        // Select the +-20000dps range
        settings.gyro.fs = ICM20948_GYRO_FS_SEL_2000DPS;
        // Enable the Accel
        settings.accel.en = ICM20948_MOD_ENABLED;
        // Select the +-2G range
        settings.accel.fs = ICM20948_ACCEL_FS_SEL_2G;
        ret = icm20948_applySettings(&settings);
    }

    // while (1)
    // {
    //     // Retrieve the Gyro data and store it in our gyro_data struct
    //     // Output is in dps (Degress per second)
    //     ret |= icm20948_getGyroData(&gyro_data);
    //     // Retrieve the Accel data and store it in our accel_data struct
    //     // Output is in mG
    //     ret |= icm20948_getAccelData(&accel_data);
    // }
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
        beep_once(30);
    }
    else if (key1_get_state() == KEY1_LONG_PRESS && last_key1_state != KEY1_LONG_PRESS)
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
