#include "service_battery.h"

/* private */

// 计算电池电压的平均值(mv)
uint16_t avg_voltage = 0;
// 剩余计算时间
int16_t remain_time = BATTERY_COMPUTE_AVG_INTERVAL;
// 已求和
uint32_t sum_adc_val = 0;
// 已计算次数
uint16_t count = 0;

// 检查电池电量
void battery_check_voltage()
{
    if (avg_voltage > 13000)
    {
        // 高压预警
        beep_once(160);
        led_flash_with_interval(4, 40);
        beep_once(160);
        led_flash_with_interval(4, 40);
    }
    else if (avg_voltage < 9600)
    {
        // 低压预警
        beep_once(300);
        led_flash_with_interval(2, 100);
    }
}

/* public */
void battery_on_time(uint16_t interval)
{
    if (remain_time > 0)
    {
        remain_time -= interval;
        count++;
        sum_adc_val += adc_adc1_get();
    }
    else
    {
        avg_voltage = (sum_adc_val / count) * 3.2468F; // val * (3.30f / 4096.0f * 4.03f) * 1000 = mV  实际值会略小
        remain_time = BATTERY_COMPUTE_AVG_INTERVAL;
        sum_adc_val = 0;
        count = 0;
        battery_check_voltage(); //  检查电池电量
    }
}

uint16_t battery_get_voltage()
{
    return avg_voltage;
}
