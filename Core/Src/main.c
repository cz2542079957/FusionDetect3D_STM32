#include "main.h"

uint16_t main_tick = 0;

int main(void)
{
    HAL_Init();           // 初始化HAL库
    SystemClock_Config(); // 初始化系统时钟
    bsp_init();           // 板载硬件初始化

    // beep_sound_start();
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

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}
