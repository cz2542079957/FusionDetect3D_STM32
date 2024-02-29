#include "bsp.h"
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

void bsp_init()
{
    return_code_t ret = RETURN_OK;
    // 串口通讯初始化
    uart_init();
    // ADC初始化
    adc_init();
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
    // 舵机初始化
    steering_engine_init();

    steering_engine_set_angle(STEERING_ENGINE_1, 50);

    // icm20948初始化
    // ret = icm20948_init();

    // while (1)
    // {
    //     uint16_t count = icm20948_get_fifo_count();
    //     // HAL_Delay(10);
    //     // // uint8_t *data;
    //     // // data = icm20948_read_fifo(count);
    //     // uint8_t data1 = icm20948_read();
    //     // HAL_Delay(10);
    //     printf("debug:%d\n", count);
    //     // if (count > 0)
    //     // {
    //     //     beep_once(10);
    //     // }
    //     HAL_Delay(10);
    // }
}
