/**
 * ===============================================================================================
 * @addtogroup icm20948
 * @{
 * ------------------------------------------------------------------------------------------------
 * @file    icm20948.h
 * @author  Pascal-Emmanuel Lachance
 * @p       <a href="https://www.github.com/Raesangur">Raesangur</a>
 * @p       <a href="https://www.raesangur.com/">https://www.raesangur.com/</a>
 *
 * @author Wolfgang (Wolle) Ewald
 * @p      <a href="https://github.com/wollewald">Wolle</a>
 * @p      <a href="https://wolles-elektronikkiste.de/en">https://wolles-elektronikkiste.de/en</a>
 *
 * @brief   A translated version of <a href="https://github.com/wollewald/ICM20948_WE/tree/main">
 *          Wolle's ICM20948 library for Arduino</a> for the ESP32 platform.
 *          This is a library for the 9-axis gyroscope, accelerometer and magnetometer ICM20948.
 *
 * @details More information can be found at
 *          <a href="https://wolles-elektronikkiste.de/en/icm-20948-9-axis-sensor-part-i">
 *          https://wolles-elektronikkiste.de/en/icm-20948-9-axis-sensor-part-i</a>
 * @details More information can be found at
 *          <a href="https://invensense.tdk.com/products/motion-tracking/9-axis/icm-20948/">
 *          https://invensense.tdk.com/products/motion-tracking/9-axis/icm-20948/</a>
 *
 * ------------------------------------------------------------------------------------------------
 * @copyright Copyright (c) 2024 Pascal-Emmanuel Lachance | Raesangur
 *                          2021 Wolfgang (Wolle) Ewald
 *
 * @par License: <a href="https://opensource.org/license/mit/"> MIT </a>
 *               This project is released under the MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * The original work was also licensed under the MIT License, and as such, is modifiable and
 * and redistributable.
 * ------------------------------------------------------------------------------------------------
 * File History:
 * @version 0.1
 * 2024-03-07- Raesangur
 *      - Initial implementation
 * ===============================================================================================
 */

/** ===============================================================================================
 *  INCLUDES
 */
#include "icm20948.h"

#include <cstdint>


/** ===============================================================================================
 *  CONSTANTS
 */


static constexpr std::uint8_t AK09916_ADDRESS{0x0C};

/* Registers ICM20948 USER BANK 0*/
static constexpr std::uint8_t ICM20948_WHO_AM_I{0x00};
static constexpr std::uint8_t ICM20948_USER_CTRL{0x03};
static constexpr std::uint8_t ICM20948_LP_CONFIG{0x05};
static constexpr std::uint8_t ICM20948_PWR_MGMT_1{0x06};
static constexpr std::uint8_t ICM20948_PWR_MGMT_2{0x07};
static constexpr std::uint8_t ICM20948_INT_PIN_CFG{0x0F};
static constexpr std::uint8_t ICM20948_INT_ENABLE{0x10};
static constexpr std::uint8_t ICM20948_INT_ENABLE_1{0x11};
static constexpr std::uint8_t ICM20948_INT_ENABLE_2{0x12};
static constexpr std::uint8_t ICM20948_INT_ENABLE_3{0x13};
static constexpr std::uint8_t ICM20948_I2C_MST_STATUS{0x17};
static constexpr std::uint8_t ICM20948_INT_STATUS{0x19};
static constexpr std::uint8_t ICM20948_INT_STATUS_1{0x1A};
static constexpr std::uint8_t ICM20948_INT_STATUS_2{0x1B};
static constexpr std::uint8_t ICM20948_INT_STATUS_3{0x1C};
static constexpr std::uint8_t ICM20948_DELAY_TIME_H{0x28};
static constexpr std::uint8_t ICM20948_DELAY_TIME_L{0x29};
static constexpr std::uint8_t ICM20948_ACCEL_OUT{0x2D};        // accel data registers begin
static constexpr std::uint8_t ICM20948_GYRO_OUT{0x33};         // gyro data registers begin
static constexpr std::uint8_t ICM20948_TEMP_OUT{0x39};
static constexpr std::uint8_t ICM20948_EXT_SLV_SENS_DATA_00{0x3B};
static constexpr std::uint8_t ICM20948_EXT_SLV_SENS_DATA_01{0x3C};
static constexpr std::uint8_t ICM20948_FIFO_EN_1{0x66};
static constexpr std::uint8_t ICM20948_FIFO_EN_2{0x67};
static constexpr std::uint8_t ICM20948_FIFO_RST{0x68};
static constexpr std::uint8_t ICM20948_FIFO_MODE{0x69};
static constexpr std::uint8_t ICM20948_FIFO_COUNT{0x70};
static constexpr std::uint8_t ICM20948_FIFO_R_W{0x72};
static constexpr std::uint8_t ICM20948_DATA_RDY_STATUS{0x74};
static constexpr std::uint8_t ICM20948_FIFO_CFG{0x76};

/* Registers ICM20948 USER BANK 1*/
static constexpr std::uint8_t ICM20948_SELF_TEST_X_GYRO{0x02};
static constexpr std::uint8_t ICM20948_SELF_TEST_Y_GYRO{0x03};
static constexpr std::uint8_t ICM20948_SELF_TEST_Z_GYRO{0x04};
static constexpr std::uint8_t ICM20948_SELF_TEST_X_ACCEL{0x0E};
static constexpr std::uint8_t ICM20948_SELF_TEST_Y_ACCEL{0x0F};
static constexpr std::uint8_t ICM20948_SELF_TEST_Z_ACCEL{0x10};
static constexpr std::uint8_t ICM20948_XA_OFFS_H{0x14};
static constexpr std::uint8_t ICM20948_XA_OFFS_L{0x15};
static constexpr std::uint8_t ICM20948_YA_OFFS_H{0x17};
static constexpr std::uint8_t ICM20948_YA_OFFS_L{0x18};
static constexpr std::uint8_t ICM20948_ZA_OFFS_H{0x1A};
static constexpr std::uint8_t ICM20948_ZA_OFFS_L{0x1B};
static constexpr std::uint8_t ICM20948_TIMEBASE_CORR_PLL{0x28};

/* Registers ICM20948 USER BANK 2*/
static constexpr std::uint8_t ICM20948_GYRO_SMPLRT_DIV{0x00};
static constexpr std::uint8_t ICM20948_GYRO_CONFIG_1{0x01};
static constexpr std::uint8_t ICM20948_GYRO_CONFIG_2{0x02};
static constexpr std::uint8_t ICM20948_XG_OFFS_USRH{0x03};
static constexpr std::uint8_t ICM20948_XG_OFFS_USRL{0x04};
static constexpr std::uint8_t ICM20948_YG_OFFS_USRH{0x05};
static constexpr std::uint8_t ICM20948_YG_OFFS_USRL{0x06};
static constexpr std::uint8_t ICM20948_ZG_OFFS_USRH{0x07};
static constexpr std::uint8_t ICM20948_ZG_OFFS_USRL{0x08};
static constexpr std::uint8_t ICM20948_ODR_ALIGN_EN{0x09};
static constexpr std::uint8_t ICM20948_ACCEL_SMPLRT_DIV_1{0x10};
static constexpr std::uint8_t ICM20948_ACCEL_SMPLRT_DIV_2{0x11};
static constexpr std::uint8_t ICM20948_ACCEL_INTEL_CTRL{0x12};
static constexpr std::uint8_t ICM20948_ACCEL_WOM_THR{0x13};
static constexpr std::uint8_t ICM20948_ACCEL_CONFIG{0x14};
static constexpr std::uint8_t ICM20948_ACCEL_CONFIG_2{0x15};
static constexpr std::uint8_t ICM20948_FSYNC_CONFIG{0x52};
static constexpr std::uint8_t ICM20948_TEMP_CONFIG{0x53};
static constexpr std::uint8_t ICM20948_MOD_CTRL_USR{0x54};

/* Registers ICM20948 USER BANK 3*/
static constexpr std::uint8_t ICM20948_I2C_MST_ODR_CFG{0x00};
static constexpr std::uint8_t ICM20948_I2C_MST_CTRL{0x01};
static constexpr std::uint8_t ICM20948_I2C_MST_DELAY_CTRL{0x02};
static constexpr std::uint8_t ICM20948_I2C_SLV0_ADDR{0x03};
static constexpr std::uint8_t ICM20948_I2C_SLV0_REG{0x04};
static constexpr std::uint8_t ICM20948_I2C_SLV0_CTRL{0x05};
static constexpr std::uint8_t ICM20948_I2C_SLV0_DO{0x06};

/* Registers ICM20948 ALL BANKS */
static constexpr std::uint8_t ICM20948_REG_BANK_SEL{0x7F};

/* Registers AK09916 */
static constexpr std::uint8_t AK09916_WIA_1{0x00};        // Who I am, Company ID
static constexpr std::uint8_t AK09916_WIA_2{0x01};        // Who I am, Device ID
static constexpr std::uint8_t AK09916_STATUS_1{0x10};
static constexpr std::uint8_t AK09916_HXL{0x11};
static constexpr std::uint8_t AK09916_HXH{0x12};
static constexpr std::uint8_t AK09916_HYL{0x13};
static constexpr std::uint8_t AK09916_HYH{0x14};
static constexpr std::uint8_t AK09916_HZL{0x15};
static constexpr std::uint8_t AK09916_HZH{0x16};
static constexpr std::uint8_t AK09916_STATUS_2{0x18};
static constexpr std::uint8_t AK09916_CNTL_2{0x31};
static constexpr std::uint8_t AK09916_CNTL_3{0x32};

/* Register Bits */
static constexpr std::uint8_t ICM20948_RESET{0x80};
static constexpr std::uint8_t ICM20948_I2C_MST_EN{0x20};
static constexpr std::uint8_t ICM20948_SLEEP{0x40};
static constexpr std::uint8_t ICM20948_LP_EN{0x20};
static constexpr std::uint8_t ICM20948_BYPASS_EN{0x02};
static constexpr std::uint8_t ICM20948_GYR_EN{0x07};
static constexpr std::uint8_t ICM20948_ACC_EN{0x38};
static constexpr std::uint8_t ICM20948_FIFO_EN{0x40};
static constexpr std::uint8_t ICM20948_INT1_ACTL{0x80};
static constexpr std::uint8_t ICM20948_INT_1_LATCH_EN{0x20};
static constexpr std::uint8_t ICM20948_ACTL_FSYNC{0x08};
static constexpr std::uint8_t ICM20948_INT_ANYRD_2CLEAR{0x10};
static constexpr std::uint8_t ICM20948_FSYNC_INT_MODE_EN{0x06};
static constexpr std::uint8_t AK09916_16_BIT{0x10};
static constexpr std::uint8_t AK09916_OVF{0x08};
static constexpr std::uint8_t AK09916_READ{0x80};

/* Others */
static constexpr std::uint16_t AK09916_WHO_AM_I_1{0x4809};
static constexpr std::uint16_t AK09916_WHO_AM_I_2{0x0948};
static constexpr std::uint8_t  ICM20948_WHO_AM_I_CONTENT{0xEA};
static constexpr float         ICM20948_ROOM_TEMP_OFFSET{0.0};
static constexpr float         ICM20948_T_SENSITIVITY{333.87};
static constexpr float         AK09916_MAG_LSB{0.1495};


/** ===============================================================================================
 *  PUBLIC METHOD DEFINITIONS
 */

icm20948::icm20948(spi_device_handle_t spi, const chip_selector<>& cs) : spi{spi}, cs{cs}
{
}


/** ===============================================================================================
 *  PROTECTED METHOD DEFINITIONS
 */
void icm20948::reset()
{
}

vec3<> icm20948::correct_acc_raw_values(vec3<> accRawVal)
{
}

vec3<> icm20948::correct_gyr_raw_values(vec3<> gyrRawVal)
{
}

vec3<> icm20948::read_xyz_val_from_fifo()
{
}

void icm20948::enable_mag_data_read(std::uint8_t reg, std::uint8_t bytes)
{
}

void icm20948::set_clock_to_auto_select()
{
}

void icm20948::switch_bank(std::uint8_t newBank)
{
}

void icm20948::write_register8(std::uint8_t bank, std::uint8_t reg, std::uint8_t val)
{
}

void icm20948::write_register16(std::uint8_t bank, std::uint8_t reg, std::int16_t val)
{
}

std::uint8_t icm20948::read_register8(std::uint8_t bank, std::uint8_t reg)
{
}

std::int16_t icm20948::read_register16(std::uint8_t bank, std::uint8_t reg)
{
}

void icm20948::read_all_data(std::uint8_t* data)
{
}

void icm20948::write_ak09916_register8(std::uint8_t reg, std::uint8_t val)
{
}

std::uint8_t icm20948::read_ak09916_register8(std::uint8_t reg)
{
}

std::int16_t icm20948::read_ak09916_register16(std::uint8_t reg)
{
}


/**
 * ------------------------------------------------------------------------------------------------
 * @}
 */
