#include "service_battery.h"

// 计算电池电压的平均值
uint16_t avg_voltage = 0;
// 剩余计算时间
int16_t remain_time = BATTERY_COMPUTE_AVG_INTERVAL;
// 已求和
uint32_t sum_adc_val = 0;
// 已计算次数
uint16_t count = 0;

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
    }
}

uint16_t battery_get_voltage()
{
    return avg_voltage;
}
