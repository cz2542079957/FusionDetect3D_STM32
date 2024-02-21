#pragma once
#include "common.h"

extern SPI_HandleTypeDef hspi2;

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
