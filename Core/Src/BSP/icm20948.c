#include <string.h>
#include "icm20948.h"
#include "icm20948_dmp.h"

static icm20948_dev_t dev;
static icm20948_settings_t settings;

uint8_t dmp3_image[] = {
#include "icm20948_dmp3a.h"
};

return_code_t icm20948_select_bank(icm20948_reg_bank_sel_t reg_bank_sel)
{
    return_code_t ret = RETURN_OK;
    if (dev.usr_bank.reg_bank_sel != reg_bank_sel)
    {
        dev.usr_bank.reg_bank_sel = reg_bank_sel;
        ret = icm20948_spi_write(ICM20948_ADDR_REG_BANK_SEL, (uint8_t *)&dev.usr_bank.reg_bank_sel, 0x01);
    }
    return ret;
}

return_code_t icm20948_i2c_controller_configure_peripheral(uint8_t peripheral, uint8_t addr, uint8_t reg, uint8_t len,
                                                           bool Rw, bool enable, bool data_only, bool grp, bool swap, uint8_t data_out)
{
    return_code_t retval = RETURN_OK;

    uint8_t periph_addr_reg;
    uint8_t periph_reg_reg;
    uint8_t periph_ctrl_reg;
    uint8_t periph_do_reg;

    switch (peripheral)
    {
    case 0:
        periph_addr_reg = ICM20948_ADDR_I2C_SLV0_ADDR;
        periph_reg_reg = ICM20948_ADDR_I2C_SLV0_REG;
        periph_ctrl_reg = ICM20948_ADDR_I2C_SLV0_CTRL;
        periph_do_reg = ICM20948_ADDR_I2C_SLV0_DO;
        break;
    case 1:
        periph_addr_reg = ICM20948_ADDR_I2C_SLV1_ADDR;
        periph_reg_reg = ICM20948_ADDR_I2C_SLV1_REG;
        periph_ctrl_reg = ICM20948_ADDR_I2C_SLV1_CTRL;
        periph_do_reg = ICM20948_ADDR_I2C_SLV1_DO;
        break;
    case 2:
        periph_addr_reg = ICM20948_ADDR_I2C_SLV2_ADDR;
        periph_reg_reg = ICM20948_ADDR_I2C_SLV2_REG;
        periph_ctrl_reg = ICM20948_ADDR_I2C_SLV2_CTRL;
        periph_do_reg = ICM20948_ADDR_I2C_SLV2_DO;
        break;
    case 3:
        periph_addr_reg = ICM20948_ADDR_I2C_SLV3_ADDR;
        periph_reg_reg = ICM20948_ADDR_I2C_SLV3_REG;
        periph_ctrl_reg = ICM20948_ADDR_I2C_SLV3_CTRL;
        periph_do_reg = ICM20948_ADDR_I2C_SLV3_DO;
        break;
    default:
        break;
    }

    retval = icm20948_select_bank(ICM20948_USER_BANK_3);
    if (retval != RETURN_OK)
    {
        return retval;
    }

    uint8_t data_reg = (Rw << 7) + addr;
    retval = icm20948_spi_write(periph_addr_reg, &data_reg, 1);
    if (retval != RETURN_OK)
    {
        return retval;
    }

    // If we are setting up a write, configure the Data Out register too
    if (!Rw)
    {
        retval = icm20948_spi_write(periph_do_reg, &data_out, 1);
        if (retval != RETURN_OK)
        {
            return retval;
        }
    }

    // Set the peripheral sub-address (register address)
    retval = icm20948_spi_write(periph_reg_reg, &reg, 1);
    if (retval != RETURN_OK)
    {
        return retval;
    }

    // Set up the control info
    uint8_t data_ctrl_reg = len + (grp << 4) + (data_only << 5) + (swap << 6) + (enable << 7);
    retval = icm20948_spi_write(periph_ctrl_reg, &data_ctrl_reg, 1);
    if (retval != RETURN_OK)
    {
        return retval;
    }
    return retval;
}

uint8_t sensor_type_2_android_sensor(enum inv_icm20948_sensor sensor)
{
    switch (sensor)
    {
    case INV_ICM20948_SENSOR_ACCELEROMETER:
        return ANDROID_SENSOR_ACCELEROMETER; // 1
    case INV_ICM20948_SENSOR_GYROSCOPE:
        return ANDROID_SENSOR_GYROSCOPE; // 4
    case INV_ICM20948_SENSOR_RAW_ACCELEROMETER:
        return ANDROID_SENSOR_RAW_ACCELEROMETER; // 42
    case INV_ICM20948_SENSOR_RAW_GYROSCOPE:
        return ANDROID_SENSOR_RAW_GYROSCOPE; // 43
    case INV_ICM20948_SENSOR_MAGNETIC_FIELD_UNCALIBRATED:
        return ANDROID_SENSOR_MAGNETIC_FIELD_UNCALIBRATED; // 14
    case INV_ICM20948_SENSOR_GYROSCOPE_UNCALIBRATED:
        return ANDROID_SENSOR_GYROSCOPE_UNCALIBRATED; // 16
    case INV_ICM20948_SENSOR_ACTIVITY_CLASSIFICATON:
        return ANDROID_SENSOR_ACTIVITY_CLASSIFICATON; // 47
    case INV_ICM20948_SENSOR_STEP_DETECTOR:
        return ANDROID_SENSOR_STEP_DETECTOR; // 18
    case INV_ICM20948_SENSOR_STEP_COUNTER:
        return ANDROID_SENSOR_STEP_COUNTER; // 19
    case INV_ICM20948_SENSOR_GAME_ROTATION_VECTOR:
        return ANDROID_SENSOR_GAME_ROTATION_VECTOR; // 15
    case INV_ICM20948_SENSOR_ROTATION_VECTOR:
        return ANDROID_SENSOR_ROTATION_VECTOR; // 11
    case INV_ICM20948_SENSOR_GEOMAGNETIC_ROTATION_VECTOR:
        return ANDROID_SENSOR_GEOMAGNETIC_ROTATION_VECTOR; // 20
    case INV_ICM20948_SENSOR_GEOMAGNETIC_FIELD:
        return ANDROID_SENSOR_GEOMAGNETIC_FIELD; // 2
    case INV_ICM20948_SENSOR_WAKEUP_SIGNIFICANT_MOTION:
        return ANDROID_SENSOR_WAKEUP_SIGNIFICANT_MOTION; // 17
    case INV_ICM20948_SENSOR_FLIP_PICKUP:
        return ANDROID_SENSOR_FLIP_PICKUP; // 46
    case INV_ICM20948_SENSOR_WAKEUP_TILT_DETECTOR:
        return ANDROID_SENSOR_WAKEUP_TILT_DETECTOR; // 41
    case INV_ICM20948_SENSOR_GRAVITY:
        return ANDROID_SENSOR_GRAVITY; // 9
    case INV_ICM20948_SENSOR_LINEAR_ACCELERATION:
        return ANDROID_SENSOR_LINEAR_ACCELERATION; // 10
    case INV_ICM20948_SENSOR_ORIENTATION:
        return ANDROID_SENSOR_ORIENTATION; // 3
    case INV_ICM20948_SENSOR_B2S:
        return ANDROID_SENSOR_B2S; // 45
    default:
        return ANDROID_SENSOR_NUM_MAX;
    }
}

enum inv_icm20948_sensor inv_icm20948_sensor_android_2_sensor_type(int sensor)
{
    switch (sensor)
    {
    case ANDROID_SENSOR_ACCELEROMETER:
        return INV_ICM20948_SENSOR_ACCELEROMETER;
    case ANDROID_SENSOR_GYROSCOPE:
        return INV_ICM20948_SENSOR_GYROSCOPE;
    case ANDROID_SENSOR_RAW_ACCELEROMETER:
        return INV_ICM20948_SENSOR_RAW_ACCELEROMETER;
    case ANDROID_SENSOR_RAW_GYROSCOPE:
        return INV_ICM20948_SENSOR_RAW_GYROSCOPE;
    case ANDROID_SENSOR_MAGNETIC_FIELD_UNCALIBRATED:
        return INV_ICM20948_SENSOR_MAGNETIC_FIELD_UNCALIBRATED;
    case ANDROID_SENSOR_GYROSCOPE_UNCALIBRATED:
        return INV_ICM20948_SENSOR_GYROSCOPE_UNCALIBRATED;
    case ANDROID_SENSOR_ACTIVITY_CLASSIFICATON:
        return INV_ICM20948_SENSOR_ACTIVITY_CLASSIFICATON;
    case ANDROID_SENSOR_STEP_DETECTOR:
        return INV_ICM20948_SENSOR_STEP_DETECTOR;
    case ANDROID_SENSOR_STEP_COUNTER:
        return INV_ICM20948_SENSOR_STEP_COUNTER;
    case ANDROID_SENSOR_GAME_ROTATION_VECTOR:
        return INV_ICM20948_SENSOR_GAME_ROTATION_VECTOR;
    case ANDROID_SENSOR_ROTATION_VECTOR:
        return INV_ICM20948_SENSOR_ROTATION_VECTOR;
    case ANDROID_SENSOR_GEOMAGNETIC_ROTATION_VECTOR:
        return INV_ICM20948_SENSOR_GEOMAGNETIC_ROTATION_VECTOR;
    case ANDROID_SENSOR_GEOMAGNETIC_FIELD:
        return INV_ICM20948_SENSOR_GEOMAGNETIC_FIELD;
    case ANDROID_SENSOR_WAKEUP_SIGNIFICANT_MOTION:
        return INV_ICM20948_SENSOR_WAKEUP_SIGNIFICANT_MOTION;
    case ANDROID_SENSOR_FLIP_PICKUP:
        return INV_ICM20948_SENSOR_FLIP_PICKUP;
    case ANDROID_SENSOR_WAKEUP_TILT_DETECTOR:
        return INV_ICM20948_SENSOR_WAKEUP_TILT_DETECTOR;
    case ANDROID_SENSOR_GRAVITY:
        return INV_ICM20948_SENSOR_GRAVITY;
    case ANDROID_SENSOR_LINEAR_ACCELERATION:
        return INV_ICM20948_SENSOR_LINEAR_ACCELERATION;
    case ANDROID_SENSOR_ORIENTATION:
        return INV_ICM20948_SENSOR_ORIENTATION;
    case ANDROID_SENSOR_B2S:
        return INV_ICM20948_SENSOR_B2S;
    default:
        return INV_ICM20948_SENSOR_MAX;
    }
}

return_code_t icm20948_dmp_init(void)
{
    return_code_t ret = RETURN_OK;
    // 设置开始地址
    ret = icm20948_select_bank(2);
    uint8_t start_address[2];
    start_address[0] = (unsigned char)(DMP_START_ADDRESS >> 8);
    start_address[1] = (unsigned char)(DMP_START_ADDRESS & 0xff);
    ret = icm20948_spi_write(ICM20948_ADDR_PRGM_START_ADDRH, &start_address, 2);

    ret = icm20948_dmp_firmware_load();

    // 设置开始地址
    ret = icm20948_select_bank(2);
    start_address[0] = (unsigned char)(DMP_START_ADDRESS >> 8);
    start_address[1] = (unsigned char)(DMP_START_ADDRESS & 0xff);
    ret = icm20948_spi_write(ICM20948_ADDR_PRGM_START_ADDRH, &start_address, 2);

    ret = icm20948_select_bank(0);
    // Set the Hardware Fix Disable register to 0x48
    ret = icm20948_spi_write(ICM20948_ADDR_HW_FIX_DISABLE, 0x48, 1);
    // Set the Single FIFO Priority Select register to 0xE4
    ret = icm20948_spi_write(ICM20948_ADDR_SINGLE_FIFO_PRIORITY_SEL, 0xE4, 1);

    // Configure Accel scaling to DMP
    // The DMP scales accel raw data internally to align 1g as 2^25
    // In order to align internal accel raw data 2^25 = 1g write 0x04000000 when
    // FSR is 4g
    const unsigned char accScale[4] = {0x04, 0x00, 0x00, 0x00};
    ret = icm20948_dmp_mems_write(ACC_SCALE, &accScale[0], 4);

    // In order to output hardware unit data as configured FSR write 0x00040000
    // when FSR is 4g
    const unsigned char accScale2[4] = {0x00, 0x04, 0x00, 0x00};
    ret = icm20948_dmp_mems_write(ACC_SCALE2, &accScale2[0], 4);

    // Configure Compass mount matrix and scale to DMP
    // The mount matrix write to DMP register is used to align the compass axes
    // with accel/gyro. This mechanism is also used to convert hardware unit to
    // uT. The value is expressed as 1uT = 2^30. Each compass axis will be
    // converted as below: X = raw_x * CPASS_MTX_00 + raw_y * CPASS_MTX_01 + raw_z
    // * CPASS_MTX_02 Y = raw_x * CPASS_MTX_10 + raw_y * CPASS_MTX_11 + raw_z *
    // CPASS_MTX_12 Z = raw_x * CPASS_MTX_20 + raw_y * CPASS_MTX_21 + raw_z *
    // CPASS_MTX_22 The AK09916 produces a 16-bit signed output in the range
    // +/-32752 corresponding to +/-4912uT. 1uT = 6.66 ADU. 2^30 / 6.66666 =
    // 161061273 = 0x9999999
    const unsigned char mountMultiplierZero[4] = {0x00, 0x00, 0x00, 0x00};
    const unsigned char mountMultiplierPlus[4] = {0x09, 0x99, 0x99, 0x99};  // Value taken from InvenSense Nucleo example
    const unsigned char mountMultiplierMinus[4] = {0xF6, 0x66, 0x66, 0x67}; // Value taken from InvenSense Nucleo example
    ret = icm20948_dmp_mems_write(CPASS_MTX_00, &mountMultiplierPlus[0], 4);
    ret = icm20948_dmp_mems_write(CPASS_MTX_01, &mountMultiplierZero[0], 4);
    ret = icm20948_dmp_mems_write(CPASS_MTX_02, &mountMultiplierZero[0], 4);
    ret = icm20948_dmp_mems_write(CPASS_MTX_10, &mountMultiplierZero[0], 4);
    ret = icm20948_dmp_mems_write(CPASS_MTX_11, &mountMultiplierMinus[0], 4);
    ret = icm20948_dmp_mems_write(CPASS_MTX_12, &mountMultiplierZero[0], 4);
    ret = icm20948_dmp_mems_write(CPASS_MTX_20, &mountMultiplierZero[0], 4);
    ret = icm20948_dmp_mems_write(CPASS_MTX_21, &mountMultiplierZero[0], 4);
    ret = icm20948_dmp_mems_write(CPASS_MTX_22, &mountMultiplierMinus[0], 4);

    // Configure the B2S Mounting Matrix
    const unsigned char b2sMountMultiplierZero[4] = {0x00, 0x00, 0x00, 0x00};
    const unsigned char b2sMountMultiplierPlus[4] = {0x40, 0x00, 0x00, 0x00}; // Value taken from InvenSense Nucleo example

    ret = icm20948_dmp_mems_write(B2S_MTX_00, &b2sMountMultiplierPlus[0], 4);
    ret = icm20948_dmp_mems_write(B2S_MTX_01, &b2sMountMultiplierZero[0], 4);
    ret = icm20948_dmp_mems_write(B2S_MTX_02, &b2sMountMultiplierZero[0], 4);
    ret = icm20948_dmp_mems_write(B2S_MTX_10, &b2sMountMultiplierZero[0], 4);
    ret = icm20948_dmp_mems_write(B2S_MTX_11, &b2sMountMultiplierPlus[0], 4);
    ret = icm20948_dmp_mems_write(B2S_MTX_12, &b2sMountMultiplierZero[0], 4);
    ret = icm20948_dmp_mems_write(B2S_MTX_20, &b2sMountMultiplierZero[0], 4);
    ret = icm20948_dmp_mems_write(B2S_MTX_21, &b2sMountMultiplierZero[0], 4);
    ret = icm20948_dmp_mems_write(B2S_MTX_22, &b2sMountMultiplierPlus[0], 4);

    // Configure the DMP Gyro Scaling Factor
    // @param[in] gyro_div Value written to GYRO_SMPLRT_DIV register, where
    //            0=1125Hz sample rate, 1=562.5Hz sample rate, ... 4=225Hz sample
    //            rate, ... 10=102.2727Hz sample rate, ... etc.
    // @param[in] gyro_level 0=250 dps, 1=500 dps, 2=1000 dps, 3=2000 dps
    ret = icm20948_dmp_set_gyro_sf(19);

    // Configure the Gyro full scale
    // 2000dps : 2^28
    // 1000dps : 2^27
    //  500dps : 2^26
    //  250dps : 2^25
    const unsigned char gyroFullScale[4] = {0x10, 0x00, 0x00, 0x00}; // 2000dps : 2^28
    ret = icm20948_dmp_mems_write(GYRO_FULLSCALE, &gyroFullScale[0], 4);

    // Configure the Accel Only Gain: 15252014 (225Hz) 30504029 (112Hz) 61117001
    // (56Hz)
    const unsigned char accelOnlyGain[4] = {0x03, 0xA4, 0x92, 0x49}; // 56Hz
    // const unsigned char accelOnlyGain[4] = {0x00, 0xE8, 0xBA, 0x2E}; // 225Hz
    // const unsigned char accelOnlyGain[4] = {0x01, 0xD1, 0x74, 0x5D}; // 112Hz
    ret = icm20948_dmp_mems_write(ACCEL_ONLY_GAIN, &accelOnlyGain[0], 4);

    // Configure the Accel Alpha Var: 1026019965 (225Hz) 977872018 (112Hz)
    // 882002213 (56Hz)
    const unsigned char accelAlphaVar[4] = {0x34, 0x92, 0x49, 0x25}; // 56Hz
    // const unsigned char accelAlphaVar[4] = {0x3D, 0x27, 0xD2, 0x7D}; // 225Hz
    // const unsigned char accelAlphaVar[4] = {0x3A, 0x49, 0x24, 0x92}; // 112Hz
    ret = icm20948_dmp_mems_write(ACCEL_ALPHA_VAR, &accelAlphaVar[0], 4);

    // Configure the Accel A Var: 47721859 (225Hz) 95869806 (112Hz) 191739611
    // (56Hz)
    const unsigned char accelAVar[4] = {0x0B, 0x6D, 0xB6, 0xDB}; // 56Hz
    // const unsigned char accelAVar[4] = {0x02, 0xD8, 0x2D, 0x83}; // 225Hz
    // const unsigned char accelAVar[4] = {0x05, 0xB6, 0xDB, 0x6E}; // 112Hz
    ret = icm20948_dmp_mems_write(ACCEL_A_VAR, &accelAVar[0], 4);

    // Configure the Accel Cal Rate
    const unsigned char accelCalRate[4] = {0x00, 0x00}; // Value taken from InvenSense Nucleo example
    ret = icm20948_dmp_mems_write(ACCEL_CAL_RATE, &accelCalRate[0], 2);

    // Configure the Compass Time Buffer. The I2C Master ODR Configuration (see
    // above) sets the magnetometer read rate to 68.75Hz. Let's set the Compass
    // Time Buffer to 69 (Hz).
    const unsigned char compassRate[2] = {0x00, 0x45}; // 69Hz
    ret = icm20948_dmp_mems_write(CPASS_TIME_BUFFER, &compassRate[0], 2);

    icm20948_dmp_enable_sensor();
    icm20948_dmp_set_sensor_period(DMP_ODR_Reg_Quat9, 0);
    icm20948_dmp_set_sensor_period(DMP_ODR_Reg_Accel, 0);
    icm20948_dmp_set_sensor_period(DMP_ODR_Reg_Gyro, 0);
    icm20948_dmp_set_sensor_period(DMP_ODR_Reg_Cpass, 0);

    ret = icm20948_select_bank(0);
    dev.usr_bank.bank0.bytes.USER_CTRL.bits.FIFO_EN = 1;
    dev.usr_bank.bank0.bytes.USER_CTRL.bits.DMP_EN = 1;
    dev.usr_bank.bank0.bytes.USER_CTRL.bits.DMP_RST = 1;
    ret = icm20948_spi_write(ICM20948_ADDR_USER_CTRL, &dev.usr_bank.bank0.bytes.USER_CTRL.byte, 0x01);

    dev.usr_bank.bank0.bytes.FIFO_RST.byte = 0x1F;
    ret = icm20948_spi_write(ICM20948_ADDR_FIFO_RST, &dev.usr_bank.bank0.bytes.FIFO_RST.byte, 0x01);

    return ret;
}

return_code_t icm20948_dmp_mems_write(uint16_t reg, uint8_t *data, uint16_t length)
{
    return_code_t ret = RETURN_OK;
    uint16_t bytes_written = 0;
    uint16_t this_length;
    uint8_t selected_mems_bank = (reg >> 8);
    uint8_t last_start_addr;

    // 设置选中的mem_bank 为 bank0
    ret = icm20948_select_bank(0);
    ret = icm20948_spi_write(ICM20948_ADDR_MEM_BANK_SEL, &selected_mems_bank, 1);

    while (bytes_written < length)
    {
        last_start_addr = (reg & 0xff);

        /* Sets the starting read or write address for the selected memory, inside of the selected page (see MEM_SEL Register).
               Contents are changed after read or write of the selected memory.
               This register must be written prior to each access to initialize the register to the proper starting address.
               The address will auto increment during burst transactions.  Two consecutive bursts without re-initializing the start address would skip one address. */

        ret = icm20948_spi_write(ICM20948_ADDR_MEM_START_ADDR, &last_start_addr, 1);
        this_length = length - bytes_written <= INV_MAX_SERIAL_WRITE ? length - bytes_written : INV_MAX_SERIAL_WRITE;

        /* Write data */
        ret = icm20948_spi_write(ICM20948_ADDR_MEM_R_W, &data[bytes_written], this_length);

        bytes_written += this_length;
        reg += this_length;
    }
    return ret;
}
return_code_t icm20948_dmp_mems_read(uint16_t reg, uint8_t *data, uint16_t length)
{
    return_code_t ret = RETURN_OK;
    uint16_t bytes_read = 0;
    uint16_t this_length;
    uint8_t selected_mems_bank = (reg >> 8);
    uint8_t last_start_addr;

    // 设置选中的mem_bank 为 bank0
    ret = icm20948_select_bank(0);
    ret = icm20948_spi_write(ICM20948_ADDR_MEM_BANK_SEL, &selected_mems_bank, 1);

    while (bytes_read < length)
    {
        last_start_addr = (reg & 0xff);

        /* Sets the starting read or write address for the selected memory, inside of the selected page (see MEM_SEL Register).
               Contents are changed after read or write of the selected memory.
               This register must be written prior to each access to initialize the register to the proper starting address.
               The address will auto increment during burst transactions.  Two consecutive bursts without re-initializing the start address would skip one address. */

        ret = icm20948_spi_write(ICM20948_ADDR_MEM_START_ADDR, &last_start_addr, 1);
        this_length = length - bytes_read <= INV_MAX_SERIAL_WRITE ? length - bytes_read : INV_MAX_SERIAL_WRITE;

        /* Write data */
        ret = icm20948_spi_read(ICM20948_ADDR_MEM_R_W, &data[bytes_read], this_length);

        bytes_read += this_length;
        reg += this_length;
    }
    return ret;
}
return_code_t icm20948_dmp_firmware_load()
{
    uint16_t addr = DMP_LOAD_START;
    uint8_t *data = dmp3_image;
    uint16_t length = sizeof(dmp3_image);
    return_code_t ret = RETURN_OK;
    int write_size;
    // Write DMP memory
    while (length > 0)
    {
        write_size = length <= INV_MAX_SERIAL_WRITE ? length : INV_MAX_SERIAL_WRITE;

        if ((addr & 0xff) + write_size > 0x100)
        {
            // Moved across a bank
            write_size = (addr & 0xff) + write_size - 0x100;
        }

        ret = icm20948_dmp_mems_write(addr, data, write_size);

        data += write_size;
        length -= write_size;
        addr += write_size;
    }
    return ret;
}
return_code_t icm20948_dmp_set_gyro_sf(uint8_t div)
{
    return_code_t ret = RETURN_OK;

    // gyro_level should be set to 4 regardless of fullscale, due to the addition of API dmp_icm20648_set_gyro_fsr()
    int32_t gyro_level = 4;

    // First read the TIMEBASE_CORRECTION_PLL register from Bank 1
    int8_t pll; // Signed. Typical value is 0x18
    ret = icm20948_select_bank(1);
    ret = icm20948_spi_read(ICM20948_ADDR_TIMEBASE_CORRECTION_PLL, &pll, 1);

    // Now calculate the Gyro SF using code taken from the InvenSense example (inv_icm20948_set_gyro_sf)
    long gyro_sf;
    unsigned long long const MagicConstant = 264446880937391LL;
    unsigned long long const MagicConstantScale = 100000LL;
    unsigned long long ResultLL;

    if (pll & 0x80)
    {
        ResultLL = (MagicConstant * (long long)(1ULL << gyro_level) * (1 + div) / (1270 - (pll & 0x7F)) / MagicConstantScale);
    }
    else
    {
        ResultLL = (MagicConstant * (long long)(1ULL << gyro_level) * (1 + div) / (1270 + pll) / MagicConstantScale);
    }
    /*
          In above deprecated FP version, worst case arguments can produce a result that overflows a signed long.
          Here, for such cases, we emulate the FP behavior of setting the result to the maximum positive value, as
          the compiler's conversion of a u64 to an s32 is simple truncation of the u64's high half, sadly....
      */
    if (ResultLL > 0x7FFFFFFF)
        gyro_sf = 0x7FFFFFFF;
    else
        gyro_sf = (long)ResultLL;

    // Finally, write the value to the DMP GYRO_SF register
    uint8_t gyro_sf_reg[4];
    gyro_sf_reg[0] = (uint8_t)(gyro_sf >> 24);
    gyro_sf_reg[1] = (uint8_t)(gyro_sf >> 16);
    gyro_sf_reg[2] = (uint8_t)(gyro_sf >> 8);
    gyro_sf_reg[3] = (uint8_t)(gyro_sf & 0xff);
    ret = icm20948_dmp_mems_write(GYRO_SF, &gyro_sf_reg, 4);
    return ret;
}
return_code_t icm20948_dmp_enable_sensor()
{
    return_code_t ret = RETURN_OK;

    uint16_t inv_event_control = 0; // Use this to store the value for MOTION_EVENT_CTL
    uint16_t data_rdy_status = 0;   // Use this to store the value for DATA_RDY_STATUS

    // uint8_t androidSensor = sensor_type_2_android_sensor(); // Convert sensor from enum inv_icm20948_sensor to Android numbering

    // Convert the Android sensor into a bit mask for DATA_OUT_CTL1
    uint16_t delta = 0;
    delta |= 0x0408;
    delta |= 0x8008;
    delta |= 0x4048;
    delta |= 0x2008;

    data_rdy_status |= DMP_Data_ready_Accel;
    inv_event_control |= DMP_Motion_Event_Control_Accel_Calibr;
    data_rdy_status |= DMP_Data_ready_Gyro;
    inv_event_control |= DMP_Motion_Event_Control_Gyro_Calibr;
    data_rdy_status |= DMP_Data_ready_Secondary_Compass;
    inv_event_control |= DMP_Motion_Event_Control_Compass_Calibr;

    // Check if Accel, Gyro/Gyro_Calibr or Compass_Calibr/Quat9/GeoMag/Compass are to be enabled. If they are then we need to request the accuracy data via header2.
    uint16_t delta2 = 0;
    if ((delta & DMP_Data_Output_Control_1_Accel) > 0)
    {
        delta2 |= DMP_Data_Output_Control_2_Accel_Accuracy;
    }
    if (((delta & DMP_Data_Output_Control_1_Gyro_Calibr) > 0) || ((delta & DMP_Data_Output_Control_1_Gyro) > 0))
    {
        delta2 |= DMP_Data_Output_Control_2_Gyro_Accuracy;
    }
    if (((delta & DMP_Data_Output_Control_1_Compass_Calibr) > 0) || ((delta & DMP_Data_Output_Control_1_Compass) > 0) || ((delta & DMP_Data_Output_Control_1_Quat9) > 0) || ((delta & DMP_Data_Output_Control_1_Geomag) > 0))
    {
        delta2 |= DMP_Data_Output_Control_2_Compass_Accuracy;
    }
    // TO DO: Add DMP_Data_Output_Control_2_Pickup etc. if required

    // Write the sensor control bits into memory address DATA_OUT_CTL1
    unsigned char data_output_control_reg[2];
    data_output_control_reg[0] = (unsigned char)(delta >> 8);
    data_output_control_reg[1] = (unsigned char)(delta & 0xff);
    ret = icm20948_dmp_mems_write(DATA_OUT_CTL1, &data_output_control_reg, 2);

    // Write the 'header2' sensor control bits into memory address DATA_OUT_CTL2
    data_output_control_reg[0] = (unsigned char)(delta2 >> 8);
    data_output_control_reg[1] = (unsigned char)(delta2 & 0xff);
    ret = icm20948_dmp_mems_write(DATA_OUT_CTL2, &data_output_control_reg, 2);

    // Set the DATA_RDY_STATUS register
    data_output_control_reg[0] = (unsigned char)(data_rdy_status >> 8);
    data_output_control_reg[1] = (unsigned char)(data_rdy_status & 0xff);
    ret = icm20948_dmp_mems_write(DMP_DATA_RDY_STATUS, &data_output_control_reg, 2);

    // Check which extra bits need to be set in the Motion Event Control register
    if ((delta & DMP_Data_Output_Control_1_Quat9) > 0)
    {
        inv_event_control |= DMP_Motion_Event_Control_9axis;
    }
    if (((delta & DMP_Data_Output_Control_1_Step_Detector) > 0) || ((delta & DMP_Data_Output_Control_1_Step_Ind_0) > 0) || ((delta & DMP_Data_Output_Control_1_Step_Ind_1) > 0) || ((delta & DMP_Data_Output_Control_1_Step_Ind_2) > 0))
    {
        inv_event_control |= DMP_Motion_Event_Control_Pedometer_Interrupt;
    }
    if ((delta & DMP_Data_Output_Control_1_Geomag) > 0)
    {
        inv_event_control |= DMP_Motion_Event_Control_Geomag;
    }

    // Set the MOTION_EVENT_CTL register
    data_output_control_reg[0] = (unsigned char)(inv_event_control >> 8);
    data_output_control_reg[1] = (unsigned char)(inv_event_control & 0xff);
    ret = icm20948_dmp_mems_write(MOTION_EVENT_CTL, &data_output_control_reg, 2);

    return ret;
}
return_code_t icm20948_dmp_set_sensor_period(enum DMP_ODR_Registers odr_reg, uint16_t interval)
{
    // Set the ODR registers and clear the ODR counter

    // In order to set an ODR for a given sensor data, write 2-byte value to DMP using key defined above for a particular sensor.
    // Setting value can be calculated as follows:
    // Value = (DMP running rate (225Hz) / ODR ) - 1
    // E.g. For a 25Hz ODR rate, value= (225/25) -1 = 8.

    // During run-time, if an ODR is changed, the corresponding rate counter must be reset.
    // To reset, write 2-byte {0,0} to DMP using keys below for a particular sensor:

    return_code_t result = RETURN_OK;
    return_code_t result2 = RETURN_OK;

    unsigned char odr_reg_val[2];
    odr_reg_val[0] = (unsigned char)(interval >> 8);
    odr_reg_val[1] = (unsigned char)(interval & 0xff);
    unsigned char odr_count_zero[2] = {0x00, 0x00};

    switch (odr_reg)
    {
    case DMP_ODR_Reg_Cpass_Calibr:
    {
        result = icm20948_dmp_mems_write(ODR_CPASS_CALIBR, &odr_reg_val, 2);
        result2 = icm20948_dmp_mems_write(ODR_CNTR_CPASS_CALIBR, &odr_count_zero, 2);
    }
    break;
    case DMP_ODR_Reg_Gyro_Calibr:
    {
        result = icm20948_dmp_mems_write(ODR_GYRO_CALIBR, &odr_reg_val, 2);
        result2 = icm20948_dmp_mems_write(ODR_CNTR_GYRO_CALIBR, &odr_count_zero, 2);
    }
    break;
    case DMP_ODR_Reg_Pressure:
    {
        result = icm20948_dmp_mems_write(ODR_PRESSURE, &odr_reg_val, 2);
        result2 = icm20948_dmp_mems_write(ODR_CNTR_PRESSURE, &odr_count_zero, 2);
    }
    break;
    case DMP_ODR_Reg_Geomag:
    {
        result = icm20948_dmp_mems_write(ODR_GEOMAG, &odr_reg_val, 2);
        result2 = icm20948_dmp_mems_write(ODR_CNTR_GEOMAG, &odr_count_zero, 2);
    }
    break;
    case DMP_ODR_Reg_PQuat6:
    {
        result = icm20948_dmp_mems_write(ODR_PQUAT6, &odr_reg_val, 2);
        result2 = icm20948_dmp_mems_write(ODR_CNTR_PQUAT6, &odr_count_zero, 2);
    }
    break;
    case DMP_ODR_Reg_Quat9:
    {
        result = icm20948_dmp_mems_write(ODR_QUAT9, &odr_reg_val, 2);
        result2 = icm20948_dmp_mems_write(ODR_CNTR_QUAT9, &odr_count_zero, 2);
    }
    break;
    case DMP_ODR_Reg_Quat6:
    {
        result = icm20948_dmp_mems_write(ODR_QUAT6, &odr_reg_val, 2);
        result2 = icm20948_dmp_mems_write(ODR_CNTR_QUAT6, &odr_count_zero, 2);
    }
    break;
    case DMP_ODR_Reg_ALS:
    {
        result = icm20948_dmp_mems_write(ODR_ALS, &odr_reg_val, 2);
        result2 = icm20948_dmp_mems_write(ODR_CNTR_ALS, &odr_count_zero, 2);
    }
    break;
    case DMP_ODR_Reg_Cpass:
    {
        result = icm20948_dmp_mems_write(ODR_CPASS, &odr_reg_val, 2);
        result2 = icm20948_dmp_mems_write(ODR_CNTR_CPASS, &odr_count_zero, 2);
    }
    break;
    case DMP_ODR_Reg_Gyro:
    {
        result = icm20948_dmp_mems_write(ODR_GYRO, &odr_reg_val, 2);
        result2 = icm20948_dmp_mems_write(ODR_CNTR_GYRO, &odr_count_zero, 2);
    }
    break;
    case DMP_ODR_Reg_Accel:
    {
        result = icm20948_dmp_mems_write(ODR_ACCEL, &odr_reg_val, 2);
        result2 = icm20948_dmp_mems_write(ODR_CNTR_ACCEL, &odr_count_zero, 2);
    }
    break;
    default:
        break;
    }

    if (result2 > result)
        result = result2;
    return result;
}
return_code_t icm20948_init()
{
    return_code_t ret = RETURN_OK;
    // SPI 初始化
    ret = icm20948_spi_init();
    /*
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
                ret = RETURN_FAIL;
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
    */

    ret = icm20948_i2c_controller_configure_peripheral(0, ICM20948_ADDR_AK09916_WHO_AM_I,
                                                       AK09916_REG_RSV2, 10, true, true, false, true, true, 0u);

    ret = icm20948_i2c_controller_configure_peripheral(
        1, ICM20948_ADDR_AK09916_WHO_AM_I, AK09916_REG_CNTL2, 1, false, true, false, false,
        false, AK09916_mode_single);
    //
    //
    //
    //
    //
    /////////////////////////////////////////////////////////////////////////////////
    ret = icm20948_select_bank(3);
    icm20948_spi_read(ICM20948_ADDR_I2C_MST_ODR_CONFIG, &dev.usr_bank.bank3.bytes.I2C_MST_ODR_CONFIG.byte, 0x01);
    uint8_t mstODRconfig = 0x04; // Set the ODR configuration to 1100/2^4 = 68.75Hz
    ret = icm20948_spi_write(ICM20948_ADDR_I2C_MST_ODR_CONFIG, &mstODRconfig, 1);
    //
    //
    //
    //
    //
    /////////////////////////////////////////////////////////////////////////////////
    ret = icm20948_select_bank(0);
    // Configure clock source through PWR_MGMT_1
    icm20948_spi_read(ICM20948_ADDR_PWR_MGMT_1, &dev.usr_bank.bank0.bytes.PWR_MGMT_1.byte, 0x01);
    dev.usr_bank.bank0.bytes.PWR_MGMT_1.bits.CLKSEL = 1;
    dev.usr_bank.bank0.bytes.PWR_MGMT_1.bits.SLEEP = 0; // 非睡眠模式
    dev.usr_bank.bank0.bytes.PWR_MGMT_1.bits.DEVICE_RESET = 0;
    ret = icm20948_spi_write(ICM20948_ADDR_PWR_MGMT_1, &dev.usr_bank.bank0.bytes.PWR_MGMT_1.byte, 0x01);

    // Enable accel and gyro sensors through PWR_MGMT_2
    icm20948_spi_read(ICM20948_ADDR_PWR_MGMT_2, &dev.usr_bank.bank0.bytes.PWR_MGMT_2.byte, 0x01);
    dev.usr_bank.bank0.bytes.PWR_MGMT_2.byte = 0x40;
    ret = icm20948_spi_write(ICM20948_ADDR_PWR_MGMT_1, &dev.usr_bank.bank0.bytes.PWR_MGMT_2.byte, 0x01);

    // Place _only_ I2C_Master in Low Power Mode (cycled) via LP_CONFIG
    // The InvenSense Nucleo example initially puts the accel and gyro into low
    // power mode too, but then later updates LP_CONFIG so only the I2C_Master is
    // in Low Power Mode
    icm20948_spi_read(ICM20948_ADDR_LP_CONFIG, &dev.usr_bank.bank0.bytes.LP_CONFIG.byte, 0x01);
    dev.usr_bank.bank0.bytes.LP_CONFIG.byte = 0;
    ret = icm20948_spi_write(ICM20948_ADDR_LP_CONFIG, &dev.usr_bank.bank0.bytes.LP_CONFIG.byte, 0x01);
    HAL_Delay(1); // issue

    // Disable the FIFO dmp
    dev.usr_bank.bank0.bytes.USER_CTRL.bits.FIFO_EN = 0;
    dev.usr_bank.bank0.bytes.USER_CTRL.bits.DMP_EN = 0;
    ret = icm20948_spi_write(ICM20948_ADDR_USER_CTRL, &dev.usr_bank.bank0.bytes.USER_CTRL.byte, 0x01);

    // Enable interrupt for FIFO overflow from FIFOs through INT_ENABLE_2
    // If we see this interrupt, we'll need to reset the FIFO
    // result = intEnableOverflowFIFO( 0x1F ); if (result > worstResult)
    // worstResult = result; // Enable the interrupt on all FIFOs
    // Turn off what goes into the FIFO through FIFO_EN_1, FIFO_EN_2
    // Stop the peripheral data from being written to the FIFO by writing zero to
    // FIFO_EN_1
    dev.usr_bank.bank0.bytes.FIFO_EN_1.byte = 0x00;
    ret = icm20948_spi_write(ICM20948_ADDR_FIFO_EN_1, &dev.usr_bank.bank0.bytes.FIFO_EN_1.byte, 0x01);
    // Stop the accelerometer, gyro and temperature data from being written to the
    // FIFO by writing zero to FIFO_EN_2
    dev.usr_bank.bank0.bytes.FIFO_EN_2.byte = 0x00;
    ret = icm20948_spi_write(ICM20948_ADDR_FIFO_EN_2, &dev.usr_bank.bank0.bytes.FIFO_EN_2.byte, 0x01);

    // Turn off data ready interrupt through INT_ENABLE_1
    ret = icm20948_spi_read(ICM20948_ADDR_INT_ENABLE_1, &dev.usr_bank.bank0.bytes.INT_ENABLE_1.byte, 0x01);
    dev.usr_bank.bank0.bytes.INT_ENABLE_1.bits.RAW_DATA_0_RDY_EN = 0;
    ret = icm20948_spi_write(ICM20948_ADDR_INT_ENABLE_1, &dev.usr_bank.bank0.bytes.INT_ENABLE_1.byte, 0x01);
    //
    //
    //
    //
    //
    /////////////////////////////////////////////////////////////////////////////////
    ret = icm20948_select_bank(2);
    // Set Gyro FSR (Full scale range) to 2000dps through GYRO_CONFIG_1
    // Set Accel FSR (Full scale range) to 4g through ACCEL_CONFIG
    // The InvenSense Nucleo code also enables the gyro DLPF (but leaves
    // GYRO_DLPFCFG set to zero = 196.6Hz (3dB)) We found this by going through
    // the SPI data generated by ZaneL's Teensy-ICM-20948 library byte by byte...
    // The gyro DLPF is enabled by default (GYRO_CONFIG_1 = 0x01) so the following
    // line should have no effect, but we'll include it anyway
    dev.usr_bank.bank2.bytes.ACCEL_CONFIG.bits.ACCEL_FS_SEL = ICM20948_ACCEL_FS_SEL_4G;
    dev.usr_bank.bank2.bytes.ACCEL_CONFIG.bits.ACCEL_FCHOICE = ICM20948_MOD_ENABLED;
    ret = icm20948_spi_write(ICM20948_ADDR_ACCEL_CONFIG, &dev.usr_bank.bank2.bytes.ACCEL_CONFIG.byte, 0x01);
    dev.usr_bank.bank2.bytes.GYRO_CONFIG_1.bits.GYRO_FS_SEL = ICM20948_GYRO_FS_SEL_2000DPS;
    dev.usr_bank.bank2.bytes.GYRO_CONFIG_1.bits.GYRO_FCHOICE = ICM20948_MOD_ENABLED;
    ret = icm20948_spi_write(ICM20948_ADDR_GYRO_CONFIG_1, &dev.usr_bank.bank2.bytes.GYRO_CONFIG_1.byte, 0x01);

    // Reset FIFO through FIFO_RST
    ret = icm20948_select_bank(0);
    dev.usr_bank.bank0.bytes.FIFO_RST.byte = 0x1F;
    ret = icm20948_spi_write(ICM20948_ADDR_FIFO_RST, &dev.usr_bank.bank0.bytes.FIFO_RST.byte, 0x01);

    ret = icm20948_select_bank(2);
    // Set gyro sample rate divider with GYRO_SMPLRT_DIV
    // Set accel sample rate divider with ACCEL_SMPLRT_DIV_2
    dev.usr_bank.bank2.bytes.ACCEL_SMPLRT_DIV_1.bits.ACCEL_SMPLRT_DIV = 0u;
    ret = icm20948_spi_write(ICM20948_ADDR_ACCEL_SMPLRT_DIV_1, &dev.usr_bank.bank2.bytes.ACCEL_SMPLRT_DIV_1.byte, 0x01);

    dev.usr_bank.bank2.bytes.ACCEL_SMPLRT_DIV_2 = (0x13 & 0xFF);
    ret = icm20948_spi_write(ICM20948_ADDR_ACCEL_SMPLRT_DIV_2, &dev.usr_bank.bank2.bytes.ACCEL_SMPLRT_DIV_2, 0x01);

    dev.usr_bank.bank2.bytes.GYRO_SMPLRT_DIV = 19;
    ret = icm20948_spi_write(ICM20948_ADDR_GYRO_SMPLRT_DIV, &dev.usr_bank.bank2.bytes.GYRO_SMPLRT_DIV, 0x01);

    ret = icm20948_dmp_init();

    return ret;
}

/*
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
*/

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
