#pragma once
#include "common.h"

extern SPI_HandleTypeDef hspi2;

#define ICM20948_SPI_GPIO_PORT GPIOB       // SPI 端口
#define ICM20948_SPI_GPIO_CS GPIO_PIN_12   // 片选引脚
#define ICM20948_SPI_GPIO_SCK GPIO_PIN_13  // SCK引脚
#define ICM20948_SPI_GPIO_MISO GPIO_PIN_14 // MISO引脚
#define ICM20948_SPI_GPIO_MOSI GPIO_PIN_15 // MOSI引脚

/**
 * @description: icm20948的spi初始化函数
 */
return_code_t icm20948_spi_init();

/**
 * @description: spi读函数
 * @param {uint8_t} addr    地址
 * @param {uint8_t} *data   读数据存储指针
 * @param {uint32_t} len    数据长度
 */
return_code_t icm20948_spi_read(uint8_t addr, uint8_t *data, uint32_t len);

/**
 * @description: spi写函数
 * @param {uint8_t} addr    地址
 * @param {uint8_t} *data   写数据存储指针
 * @param {uint32_t} len    数据长度
 */
return_code_t icm20948_spi_write(uint8_t addr, uint8_t *data, uint32_t len);
