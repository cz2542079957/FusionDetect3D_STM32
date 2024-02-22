
#include <string.h>
#include "icm20948.h"
#include "icm20948_spi.h"
#include "icm20948_api.h"

static icm20948_dev_t dev;
static icm20948_settings_t settings;

return_code_t icm20948_init()
{
    return_code_t ret = RETURN_OK;

    // SPI 初始化
    ret = icm20948_spi_init();

    dev.usr_bank.reg_bank_sel = ICM20948_USER_BANK_0;
    if (ret == RETURN_OK)
    {
        // 选择用户寄存器bank0
        ret = icm20948_spi_write(ICM20948_ADDR_REG_BANK_SEL, (uint8_t *)&dev.usr_bank.reg_bank_sel, 0x01);
    }

    if (ret == RETURN_OK)
    {
        // 初始化为0，再读取
        dev.usr_bank.bank0.bytes.WHO_AM_I = 0x00;

        // 读取WHO_AM_I寄存器
        ret = icm20948_spi_read(ICM20948_ADDR_WHO_AM_I, &dev.usr_bank.bank0.bytes.WHO_AM_I, 0x01);

        // Device ID 错误
        if (dev.usr_bank.bank0.bytes.WHO_AM_I != ICM20948_WHO_AM_I_DEFAULT)
        {
            ret = RETURN_GEN_FAIL;
        }
    }

    if (ret == RETURN_OK)
    {
        dev.usr_bank.bank0.bytes.PWR_MGMT_1.bits.CLKSEL = 1;
        dev.usr_bank.bank0.bytes.PWR_MGMT_1.bits.SLEEP = 0; // 非睡眠模式
        dev.usr_bank.bank0.bytes.PWR_MGMT_1.bits.DEVICE_RESET = 0;
        ret = icm20948_spi_write(ICM20948_ADDR_PWR_MGMT_1, &dev.usr_bank.bank0.bytes.PWR_MGMT_1.byte, 0x01);
    }

    if (ret == RETURN_OK)
    {
        icm20948_settings_t settings;
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

    return ret;
}

return_code_t icm20948_applySettings(icm20948_settings_t *newSettings)
{
    return_code_t ret = RETURN_OK;

    // Copy over the new settings
    memcpy(&settings, newSettings, sizeof(settings));

    // Apply the new settings
    if (settings.gyro.en == ICM20948_MOD_ENABLED)
    {
        // Select Bank 2 if it isn't already
        if ((dev.usr_bank.reg_bank_sel != ICM20948_USER_BANK_2) && (ret == RETURN_OK))
        {
            // Select Bank 0
            dev.usr_bank.reg_bank_sel = ICM20948_USER_BANK_2;
            // Write to the reg bank select to select bank 2
            ret = icm20948_spi_write(ICM20948_ADDR_REG_BANK_SEL, (uint8_t *)&dev.usr_bank.reg_bank_sel, 0x01);
        }

        if (ret == RETURN_OK)
        {
            // Set the Gyro Rate
            dev.usr_bank.bank2.bytes.GYRO_CONFIG_1.bits.GYRO_FCHOICE = 1;
            dev.usr_bank.bank2.bytes.GYRO_CONFIG_1.bits.GYRO_FS_SEL = settings.gyro.fs;
            dev.usr_bank.bank2.bytes.GYRO_CONFIG_1.bits.GYRO_DLPFCFG = 5;
            ret = icm20948_spi_write(ICM20948_ADDR_GYRO_CONFIG_1, &dev.usr_bank.bank2.bytes.GYRO_CONFIG_1.byte, 0x01);
        }

        if (ret == RETURN_OK)
        {
            // Set the sample rate
            dev.usr_bank.bank2.bytes.GYRO_SMPLRT_DIV = 0x0A;
            ret = icm20948_spi_write(ICM20948_ADDR_GYRO_SMPLRT_DIV, &dev.usr_bank.bank2.bytes.GYRO_SMPLRT_DIV, 0x01);
        }
    }
    else
    {
        // Disable the Gyro
        // Select Bank 0 if it isn't already
        if ((dev.usr_bank.reg_bank_sel != ICM20948_USER_BANK_0) && (ret == RETURN_OK))
        {
            // Select Bank 0
            dev.usr_bank.reg_bank_sel = ICM20948_USER_BANK_0;
            // Write to the reg bank select to select bank 0
            ret = icm20948_spi_write(ICM20948_ADDR_REG_BANK_SEL, (uint8_t *)&dev.usr_bank.reg_bank_sel, 0x01);
        }

        if (ret == RETURN_OK)
        {
            // Read out the current PWR_MGMT Byte
            ret = icm20948_spi_read(ICM20948_ADDR_PWR_MGMT_2, &dev.usr_bank.bank0.bytes.PWR_MGMT_2.byte, 0x01);
        }

        if (ret == RETURN_OK)
        {
            // Now disable the gyro in the config
            dev.usr_bank.bank0.bytes.PWR_MGMT_2.bits.DISABLE_GYRO = 0b111;
            // Write the config back to the device
            ret = icm20948_spi_write(ICM20948_ADDR_PWR_MGMT_2, &dev.usr_bank.bank0.bytes.PWR_MGMT_2.byte, 0x01);
        }
    }

    if (settings.accel.en == ICM20948_MOD_ENABLED)
    {
        // Select Bank 2 if it isn't already
        if ((dev.usr_bank.reg_bank_sel != ICM20948_USER_BANK_2) && (ret == RETURN_OK))
        {
            // Select Bank 0
            dev.usr_bank.reg_bank_sel = ICM20948_USER_BANK_2;
            // Write to the reg bank select to select bank 2
            ret = icm20948_spi_write(ICM20948_ADDR_REG_BANK_SEL, (uint8_t *)&dev.usr_bank.reg_bank_sel, 0x01);
        }

        if (ret == RETURN_OK)
        {
            // Setup the Accel Config
            dev.usr_bank.bank2.bytes.ACCEL_CONFIG.bits.ACCEL_FS_SEL = settings.accel.fs;
            dev.usr_bank.bank2.bytes.ACCEL_CONFIG.bits.ACCEL_FCHOICE = 1;
            dev.usr_bank.bank2.bytes.ACCEL_CONFIG.bits.ACCEL_DLPFCFG = 5;
            ret = icm20948_spi_write(ICM20948_ADDR_ACCEL_CONFIG, &dev.usr_bank.bank2.bytes.ACCEL_CONFIG.byte, 0x01);
        }

        if (ret == RETURN_OK)
        {
            // Set the sample rate
            dev.usr_bank.bank2.bytes.ACCEL_SMPLRT_DIV_1.bits.ACCEL_SMPLRT_DIV = 0;
            ret = icm20948_spi_write(ICM20948_ADDR_ACCEL_SMPLRT_DIV_1, &dev.usr_bank.bank2.bytes.ACCEL_SMPLRT_DIV_1.byte, 0x01);
        }

        if (ret == RETURN_OK)
        {
            // Set the sample rate
            dev.usr_bank.bank2.bytes.ACCEL_SMPLRT_DIV_2 = 0x0A;
            ret = icm20948_spi_write(ICM20948_ADDR_ACCEL_SMPLRT_DIV_2, &dev.usr_bank.bank2.bytes.ACCEL_SMPLRT_DIV_2, 0x01);
        }
    }
    else
    {
        // Disable the Accelerometer
        // Select Bank 0 if it isn't already
        if ((dev.usr_bank.reg_bank_sel != ICM20948_USER_BANK_0) && (ret == RETURN_OK))
        {
            // Select Bank 0
            dev.usr_bank.reg_bank_sel = ICM20948_USER_BANK_0;
            // Write to the reg bank select to select bank 0
            ret = icm20948_spi_write(ICM20948_ADDR_REG_BANK_SEL, (uint8_t *)&dev.usr_bank.reg_bank_sel, 0x01);
        }

        if (ret == RETURN_OK)
        {
            // Read out the current PWR_MGMT Byte
            ret = icm20948_spi_read(ICM20948_ADDR_PWR_MGMT_2, &dev.usr_bank.bank0.bytes.PWR_MGMT_2.byte, 0x01);
        }

        if (ret == RETURN_OK)
        {
            // Now disable the accel in the config
            dev.usr_bank.bank0.bytes.PWR_MGMT_2.bits.DISABLE_ACCEL = 0b111;
            // Write the config back to the device
            ret = icm20948_spi_write(ICM20948_ADDR_PWR_MGMT_2, &dev.usr_bank.bank0.bytes.PWR_MGMT_2.byte, 0x01);
        }
    }

    if ((dev.usr_bank.reg_bank_sel != ICM20948_USER_BANK_0) && (ret == RETURN_OK))
    {
        // Select Bank 0
        dev.usr_bank.reg_bank_sel = ICM20948_USER_BANK_0;
        ret = icm20948_spi_write(ICM20948_ADDR_REG_BANK_SEL, (uint8_t *)&dev.usr_bank.reg_bank_sel, 0x01);
    }

    // if (ret == RETURN_OK)
    // {
    //     // Enable the FIFO and DMP
    //     dev.usr_bank.bank0.bytes.FIFO_MODE.bits.FIFO_MODE = 0;
    //     ret = icm20948_spi_write(ICM20948_ADDR_FIFO_MODE, &dev.usr_bank.bank0.bytes.USER_CTRL.byte, 0x01);
    // }

    if (ret == RETURN_OK)
    {
        // Enable the FIFO and DMP
        dev.usr_bank.bank0.bytes.USER_CTRL.bits.FIFO_EN = 1;
        dev.usr_bank.bank0.bytes.USER_CTRL.bits.DMP_EN = 1;
        ret = icm20948_spi_write(ICM20948_ADDR_USER_CTRL, &dev.usr_bank.bank0.bytes.USER_CTRL.byte, 0x01);
    }

    if (ret == RETURN_OK)
    {
        dev.usr_bank.bank0.bytes.FIFO_EN_1.bits.SLV_0_FIFO_EN = 1;
        dev.usr_bank.bank0.bytes.FIFO_EN_1.bits.SLV_1_FIFO_EN = 1;
        dev.usr_bank.bank0.bytes.FIFO_EN_1.bits.SLV_2_FIFO_EN = 1;
        dev.usr_bank.bank0.bytes.FIFO_EN_1.bits.SLV_3_FIFO_EN = 1;
        ret = icm20948_spi_write(ICM20948_ADDR_FIFO_EN_1, &dev.usr_bank.bank0.bytes.FIFO_EN_1.byte, 0x01);
    }
    if (ret == RETURN_OK)
    {
        dev.usr_bank.bank0.bytes.FIFO_EN_2.bits.ACCEL_FIFO_EN = 1;
        dev.usr_bank.bank0.bytes.FIFO_EN_2.bits.GYRO_X_FIFO_EN = 1;
        dev.usr_bank.bank0.bytes.FIFO_EN_2.bits.GYRO_Y_FIFO_EN = 1;
        dev.usr_bank.bank0.bytes.FIFO_EN_2.bits.GYRO_Z_FIFO_EN = 1;
        dev.usr_bank.bank0.bytes.FIFO_EN_2.bits.TEMP_FIFO_EN = 1;
        ret = icm20948_spi_write(ICM20948_ADDR_FIFO_EN_2, &dev.usr_bank.bank0.bytes.FIFO_EN_2.byte, 0x01);
    }

    return ret;
}

return_code_t icm20948_getGyroData(icm20948_gyro_t *gyro)
{
    return_code_t ret = RETURN_OK;

    // Check if the Gyro is enabled
    if (settings.gyro.en != ICM20948_MOD_ENABLED)
    {
        ret = RETURN_INV_CONFIG;
    }

    if ((dev.usr_bank.reg_bank_sel != ICM20948_USER_BANK_0) && (ret == RETURN_OK))
    {
        // Select Bank 0
        dev.usr_bank.reg_bank_sel = ICM20948_USER_BANK_0;
        // Write to the reg bank select to select bank 0
        ret = icm20948_spi_write(ICM20948_ADDR_REG_BANK_SEL, (uint8_t *)&dev.usr_bank.reg_bank_sel, 0x01);
    }

    if (ret == RETURN_OK)
    {
        // Read out the 6 bytes of gyro data
        ret = icm20948_spi_read(ICM20948_ADDR_GYRO_XOUT_H, &dev.usr_bank.bank0.bytes.GYRO_XOUT_H, 0x06);
    }

    if (ret == RETURN_OK)
    {
        // Arrang the gyro data nicely in the provided struct
        gyro->x = ((int16_t)dev.usr_bank.bank0.bytes.GYRO_XOUT_H << 8) | dev.usr_bank.bank0.bytes.GYRO_XOUT_L;
        gyro->y = ((int16_t)dev.usr_bank.bank0.bytes.GYRO_YOUT_H << 8) | dev.usr_bank.bank0.bytes.GYRO_YOUT_L;
        gyro->z = ((int16_t)dev.usr_bank.bank0.bytes.GYRO_ZOUT_H << 8) | dev.usr_bank.bank0.bytes.GYRO_ZOUT_L;

        // Determine the scaling factor based on the Full scale select config
        // and then scale the values
        switch (settings.gyro.fs)
        {
        case ICM20948_GYRO_FS_SEL_250DPS:
            gyro->x /= 131;
            gyro->y /= 131;
            gyro->z /= 131;
            break;

        case ICM20948_GYRO_FS_SEL_500DPS:
            gyro->x /= 65.5;
            gyro->y /= 65.5;
            gyro->z /= 65.5;
            break;

        case ICM20948_GYRO_FS_SEL_1000DPS:
            gyro->x /= 32.8;
            gyro->y /= 32.8;
            gyro->z /= 32.8;
            break;

        case ICM20948_GYRO_FS_SEL_2000DPS:
            gyro->x /= 16.4;
            gyro->y /= 16.4;
            gyro->z /= 16.4;
            break;

        default:
            // We have an invalid config setting for the resolution
            gyro->x = 0;
            gyro->y = 0;
            gyro->z = 0;
            ret = RETURN_INV_CONFIG;
            break;
        }

        if (ret == RETURN_OK)
        {
            // Remove noise by modulo dividing with our configured
            // resolution
            gyro->x -= (gyro->x % 10);
            gyro->y -= (gyro->y % 10);
            gyro->z -= (gyro->z % 10);
        }
    }
    else
    {
        gyro->x = 0;
        gyro->y = 0;
        gyro->z = 0;
    }

    return ret;
}

return_code_t icm20948_getAccelData(icm20948_accel_t *accel)
{
    return_code_t ret = RETURN_OK;

    // Check if the Accelerometer is enabled
    if (settings.accel.en != ICM20948_MOD_ENABLED)
    {
        ret = RETURN_INV_CONFIG;
    }

    if ((dev.usr_bank.reg_bank_sel != ICM20948_USER_BANK_0) && (ret == RETURN_OK))
    {
        // Select Bank 0
        dev.usr_bank.reg_bank_sel = ICM20948_USER_BANK_0;
        // Write to the reg bank select to select bank 0
        ret = icm20948_spi_write(ICM20948_ADDR_REG_BANK_SEL, (uint8_t *)&dev.usr_bank.reg_bank_sel, 0x01);
    }

    if (ret == RETURN_OK)
    {
        // Read out the 6 bytes of gyro data
        ret = icm20948_spi_read(ICM20948_ADDR_ACCEL_XOUT_H, &dev.usr_bank.bank0.bytes.ACCEL_XOUT_H, 0x06);
    }

    if (ret == RETURN_OK)
    {
        // Arrang the gyro data nicely in the provided struct
        accel->x = ((int16_t)dev.usr_bank.bank0.bytes.ACCEL_XOUT_H << 8) | dev.usr_bank.bank0.bytes.ACCEL_XOUT_L;
        accel->y = ((int16_t)dev.usr_bank.bank0.bytes.ACCEL_YOUT_H << 8) | dev.usr_bank.bank0.bytes.ACCEL_YOUT_L;
        accel->z = ((int16_t)dev.usr_bank.bank0.bytes.ACCEL_ZOUT_H << 8) | dev.usr_bank.bank0.bytes.ACCEL_ZOUT_L;

        // Determine the scaling factor based on the Full scale select config
        // and then scale the values
        switch (settings.accel.fs)
        {
        case ICM20948_ACCEL_FS_SEL_2G:
            accel->x /= 16;
            accel->y /= 16;
            accel->z /= 16;
            break;

        case ICM20948_ACCEL_FS_SEL_4G:
            accel->x /= 8;
            accel->y /= 8;
            accel->z /= 8;
            break;

        case ICM20948_ACCEL_FS_SEL_8G:
            accel->x /= 4;
            accel->y /= 4;
            accel->z /= 4;
            break;

        case ICM20948_ACCEL_FS_SEL_16G:
            accel->x /= 2;
            accel->y /= 2;
            accel->z /= 2;
            break;

        default:
            // We have an invalid config setting for the resolution
            accel->x = 0;
            accel->y = 0;
            accel->z = 0;
            ret = RETURN_INV_CONFIG;
            break;
        }

        if (ret == RETURN_OK)
        {
            // Remove noise by modulo dividing with our configured
            // resolution
            accel->x -= (accel->x % 50);
            accel->y -= (accel->y % 50);
            accel->z -= (accel->z % 50);
        }
    }
    else
    {
        accel->x = 0;
        accel->y = 0;
        accel->z = 0;
    }

    return ret;
}

uint16_t icm20948_get_fifo_count()
{
    uint16_t count = 0x00;
    uint8_t count_l = 0x00;
    uint8_t count_h = 0x00;
    if (icm20948_spi_read(ICM20948_ADDR_FIFO_COUNTH, &count_h, 0x01) == RETURN_OK)
    {
        if (icm20948_spi_read(ICM20948_ADDR_FIFO_COUNTL, &count_l, 0x01) == RETURN_OK)
        {
            count = (count_h << 8) | count_l;
        }
    }
    return count;
}

uint8_t *icm20948_read_fifo(uint16_t len)
{
    uint8_t data[len];
    memset(data, 0, sizeof(data));
    // if (icm20948_spi_read(ICM20948_ADDR_FIFO_R_W, &data, len) == RETURN_OK)
    // {
    //     return data;
    // }
    if (icm20948_spi_read(ICM20948_ADDR_FIFO_EN_1, &data, 0x01) == RETURN_OK)
    {
        return data;
    }
}

uint8_t icm20948_read()
{
    uint8_t data;
    // if (icm20948_spi_read(ICM20948_ADDR_FIFO_EN_1, &data, 0x01) == RETURN_OK)
    // {
    //     return data;
    // }

    if (icm20948_spi_read(ICM20948_ADDR_USER_CTRL, &data, 0x01) == RETURN_OK)
    {
        return data;
    }
}
