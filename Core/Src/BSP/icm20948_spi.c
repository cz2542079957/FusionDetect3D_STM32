#include "icm20948_spi.h"

SPI_HandleTypeDef hspi2;

uint8_t icm20948_spi_init()
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    // GPIO初始化
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // cs
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // SPI初始化
    __HAL_RCC_SPI2_CLK_ENABLE();                              // 启用SPI2的时钟
    hspi2.Instance = SPI2;                                    // 设置SPI2外设实例
    hspi2.Init.Mode = SPI_MODE_MASTER;                        // 设置为SPI主模式
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;              // 设置为双线双向全双工模式
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;                  // 设置SPI数据帧大小为8位
    hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;               // 设置SCK空闲状态为高电平
    hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;                    // 设置数据在SCK第二个跳变沿（上升沿或下降沿）被采样
    hspi2.Init.NSS = SPI_NSS_SOFT;                            // 设置NSS信号由软件管理
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; // 设置波特率预分频值为256
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;                   // 设置数据传输从最高位（MSB）开始
    hspi2.Init.CRCPolynomial = 7;                             // 设置CRC多项式（通常在需要CRC校验时使用）
    HAL_SPI_Init(&hspi2);                                     // 初始化SPI2外设
}
