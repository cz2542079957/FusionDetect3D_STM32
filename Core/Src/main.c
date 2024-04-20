#include "main.h"

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

int main(void)
{
    HAL_Init();           // 初始化HAL库
    SystemClock_Config(); // 初始化系统时钟
    bsp_init();           // 板载硬件初始化

    uint16_t main_tick = 0; // 主循环计数器
    beep_sound_start();
    led_flash(20);
    led_keep_on();
    while (1)
    {
        /*
        fast循环每 FAST_LOOP_CYCLE_TICKS 个 MAIN_LOOP_CYCLE 执行一次
        normal循环每 NORMAL_LOOP_CYCLE_TICKS 个 MAIN_LOOP_CYCLE 执行一次
        slow循环每 SLOW_LOOP_CYCLE_TICKS 个 MAIN_LOOP_CYCLE 执行一次
        slowest循环每 SLOWEST_LOOP_CYCLE_TICKS 个 MAIN_LOOP_CYCLE 执行一次
        */
        fast_on_time(FAST_LOOP_CYCLE_TICKS * MAIN_LOOP_CYCLE);
        if (main_tick % NORMAL_LOOP_CYCLE_TICKS == 0)
            normal_on_time(NORMAL_LOOP_CYCLE_TICKS * MAIN_LOOP_CYCLE);
        if (main_tick % SLOW_LOOP_CYCLE_TICKS == 0)
            slow_on_time(SLOW_LOOP_CYCLE_TICKS * MAIN_LOOP_CYCLE);
        if (main_tick % SLOWEST_LOOP_CYCLE_TICKS == 0)
            slowest_on_time(SLOWEST_LOOP_CYCLE_TICKS * MAIN_LOOP_CYCLE);
        main_tick++;
        HAL_Delay(MAIN_LOOP_CYCLE - 1);
    }
    // beep_sound_end();
}

inline void fast_on_time(uint16_t interval)
{
    // 发出编码器数据
    int16_t *encoder = encoder_get_count();
    uint8_t encoder_frame[5] = {FRAME_RESPONSE_ENCODER, encoder[0], encoder[1], encoder[2], encoder[3]};
    service_protocol_send_frame(encoder_frame, 5);

    // 发出舵机数据
    float *servo = service_servo_get_all_angle();
    uint16_t servo1 = servo[0] * 10, servo2 = servo[1] * 10;
    uint8_t servo_frame[5] = {FRAME_RESPONSE_SERVO, (uint8_t)(servo1 & 0xff), (uint8_t)(servo1 >> 8), (uint8_t)(servo2 & 0xff), (uint8_t)(servo2 >> 8)}; 
    service_protocol_send_frame(servo_frame, 5);
}

inline void normal_on_time(uint16_t interval)
{
    // key1
    key1_on_time(interval);
    // led
    led_on_time(interval);
    // beep
    beep_on_time(interval);
    // motion
    motion_on_time(interval);
    // servo
    service_servo_on_time(interval);

    // protocol
    service_protocol_on_time(interval);
}

inline void slow_on_time(uint16_t interval)
{
    // 电池周期函数
    service_battery_on_time(interval);
}

inline void slowest_on_time(uint16_t interval)
{
    // 输出电压
    uint16_t v = battery_get_voltage();
    uint8_t frame_voltage[3] = {FRAME_RESPONSE_VOTAGE, v & 0xff, (v >> 8) & 0xff};
    service_protocol_send_frame(frame_voltage, 3);
}
