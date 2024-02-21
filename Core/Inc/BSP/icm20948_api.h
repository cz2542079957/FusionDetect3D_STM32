
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "icm20948_spi.h"

typedef enum
{
    ICM20948_MOD_DISABLED = 0x00,
    ICM20948_MOD_ENABLED
} icm20948_mod_enable_t;

typedef enum
{
    ICM20948_GYRO_FS_SEL_250DPS = 0x00,
    ICM20948_GYRO_FS_SEL_500DPS = 0x01,
    ICM20948_GYRO_FS_SEL_1000DPS = 0x02,
    ICM20948_GYRO_FS_SEL_2000DPS = 0x03
} icm20948_gyro_full_scale_select_t;

typedef struct
{
    icm20948_mod_enable_t en;
    icm20948_gyro_full_scale_select_t fs;
} icm20948_gyro_settings_t;

typedef enum
{
    ICM20948_ACCEL_FS_SEL_2G = 0x00,
    ICM20948_ACCEL_FS_SEL_4G = 0x01,
    ICM20948_ACCEL_FS_SEL_8G = 0x02,
    ICM20948_ACCEL_FS_SEL_16G = 0x03
} icm20948_accel_full_scale_select_t;

typedef struct
{
    icm20948_mod_enable_t en;
    icm20948_accel_full_scale_select_t fs;
} icm20948_accel_settings_t;

typedef struct
{
    icm20948_mod_enable_t en;
} icm20948_mag_settings_t;

typedef struct
{
    icm20948_gyro_settings_t gyro;
    icm20948_accel_settings_t accel;
    icm20948_mag_settings_t mag;
} icm20948_settings_t;

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
} icm20948_gyro_t;

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
} icm20948_accel_t;

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
} icm20948_mag_t;

/**
 * @description:  icm20948初始化函数
 */
return_code_t icm20948_init();

return_code_t icm20948_applySettings(icm20948_settings_t *newSettings);

return_code_t icm20948_getGyroData(icm20948_gyro_t *gyro);

return_code_t icm20948_getAccelData(icm20948_accel_t *accel);
