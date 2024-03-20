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

#include "freertos/task.h"

#include <algorithm>
#include <cstdint>


/** ===============================================================================================
 *  CONSTANTS
 */

namespace ICM20948
{

static constexpr TickType_t PERIOD_MS{10};
static constexpr TickType_t RESET_DELAY{10 / PERIOD_MS};
static constexpr TickType_t RESET_MAG_DELAY{100 / PERIOD_MS};
static constexpr TickType_t READ_DELAY{10 / PERIOD_MS};

static constexpr std::uint8_t AK09916_ADDRESS{0x0C};

/* Registers ICM20948 USER BANK 0*/
static constexpr std::uint8_t WHO_AM_I{0x00U};
static constexpr std::uint8_t USER_CTRL{0x03U};
static constexpr std::uint8_t LP_CONFIG{0x05U};
static constexpr std::uint8_t PWR_MGMT_1{0x06U};
static constexpr std::uint8_t PWR_MGMT_2{0x07U};
static constexpr std::uint8_t INT_PIN_CFG{0x0FU};
static constexpr std::uint8_t INT_ENABLE{0x10U};
static constexpr std::uint8_t INT_ENABLE_1{0x11U};
static constexpr std::uint8_t INT_ENABLE_2{0x12U};
static constexpr std::uint8_t INT_ENABLE_3{0x13U};
static constexpr std::uint8_t I2C_MST_STATUS{0x17U};
static constexpr std::uint8_t INT_STATUS{0x19U};
static constexpr std::uint8_t INT_STATUS_1{0x1AU};
static constexpr std::uint8_t INT_STATUS_2{0x1BU};
static constexpr std::uint8_t INT_STATUS_3{0x1CU};
static constexpr std::uint8_t DELAY_TIME_H{0x28U};
static constexpr std::uint8_t DELAY_TIME_L{0x29U};
static constexpr std::uint8_t ACCEL_OUT{0x2DU};        // accel data registers begin
static constexpr std::uint8_t GYRO_OUT{0x33U};         // gyro data registers begin
static constexpr std::uint8_t TEMP_OUT{0x39U};
static constexpr std::uint8_t EXT_SLV_SENS_DATA_00{0x3BU};
static constexpr std::uint8_t EXT_SLV_SENS_DATA_01{0x3CU};
static constexpr std::uint8_t FIFO_EN_1{0x66U};
static constexpr std::uint8_t FIFO_EN_2{0x67U};
static constexpr std::uint8_t FIFO_RST{0x68U};
static constexpr std::uint8_t FIFO_MODE{0x69U};
static constexpr std::uint8_t FIFO_COUNT{0x70U};
static constexpr std::uint8_t FIFO_R_W{0x72U};
static constexpr std::uint8_t DATA_RDY_STATUS{0x74U};
static constexpr std::uint8_t FIFO_CFG{0x76U};

/* Registers ICM20948 USER BANK 1*/
static constexpr std::uint8_t SELF_TEST_X_GYRO{0x02U};
static constexpr std::uint8_t SELF_TEST_Y_GYRO{0x03U};
static constexpr std::uint8_t SELF_TEST_Z_GYRO{0x04U};
static constexpr std::uint8_t SELF_TEST_X_ACCEL{0x0EU};
static constexpr std::uint8_t SELF_TEST_Y_ACCEL{0x0FU};
static constexpr std::uint8_t SELF_TEST_Z_ACCEL{0x10U};
static constexpr std::uint8_t XA_OFFS_H{0x14U};
static constexpr std::uint8_t XA_OFFS_L{0x15U};
static constexpr std::uint8_t YA_OFFS_H{0x17U};
static constexpr std::uint8_t YA_OFFS_L{0x18U};
static constexpr std::uint8_t ZA_OFFS_H{0x1AU};
static constexpr std::uint8_t ZA_OFFS_L{0x1BU};
static constexpr std::uint8_t TIMEBASE_CORR_PLL{0x28U};

/* Registers ICM20948 USER BANK 2*/
static constexpr std::uint8_t GYRO_SMPLRT_DIV{0x00U};
static constexpr std::uint8_t GYRO_CONFIG_1{0x01U};
static constexpr std::uint8_t GYRO_CONFIG_2{0x02U};
static constexpr std::uint8_t XG_OFFS_USRH{0x03U};
static constexpr std::uint8_t XG_OFFS_USRL{0x04U};
static constexpr std::uint8_t YG_OFFS_USRH{0x05U};
static constexpr std::uint8_t YG_OFFS_USRL{0x06U};
static constexpr std::uint8_t ZG_OFFS_USRH{0x07U};
static constexpr std::uint8_t ZG_OFFS_USRL{0x08U};
static constexpr std::uint8_t ODR_ALIGN_EN{0x09U};
static constexpr std::uint8_t ACCEL_SMPLRT_DIV_1{0x10U};
static constexpr std::uint8_t ACCEL_SMPLRT_DIV_2{0x11U};
static constexpr std::uint8_t ACCEL_INTEL_CTRL{0x12U};
static constexpr std::uint8_t ACCEL_WOM_THR{0x13U};
static constexpr std::uint8_t ACCEL_CONFIG{0x14U};
static constexpr std::uint8_t ACCEL_CONFIG_2{0x15U};
static constexpr std::uint8_t FSYNC_CONFIG{0x52U};
static constexpr std::uint8_t TEMP_CONFIG{0x53U};
static constexpr std::uint8_t MOD_CTRL_USR{0x54U};

/* Registers ICM20948 USER BANK 3*/
static constexpr std::uint8_t I2C_MST_ODR_CFG{0x00U};
static constexpr std::uint8_t I2C_MST_CTRL{0x01U};
static constexpr std::uint8_t I2C_MST_DELAY_CTRL{0x02U};
static constexpr std::uint8_t I2C_SLV0_ADDR{0x03U};
static constexpr std::uint8_t I2C_SLV0_REG{0x04U};
static constexpr std::uint8_t I2C_SLV0_CTRL{0x05U};
static constexpr std::uint8_t I2C_SLV0_DO{0x06U};

/* Registers ICM20948 ALL BANKS */
static constexpr std::uint8_t REG_BANK_SEL{0x7FU};

/* Registers AK09916 */
static constexpr std::uint8_t AK09916_WIA_1{0x00U};        // Who I am, Company ID
static constexpr std::uint8_t AK09916_WIA_2{0x01U};        // Who I am, Device ID
static constexpr std::uint8_t AK09916_STATUS_1{0x10U};
static constexpr std::uint8_t AK09916_HXL{0x11U};
static constexpr std::uint8_t AK09916_HXH{0x12U};
static constexpr std::uint8_t AK09916_HYL{0x13U};
static constexpr std::uint8_t AK09916_HYH{0x14U};
static constexpr std::uint8_t AK09916_HZL{0x15U};
static constexpr std::uint8_t AK09916_HZH{0x16U};
static constexpr std::uint8_t AK09916_STATUS_2{0x18U};
static constexpr std::uint8_t AK09916_CNTL_2{0x31U};
static constexpr std::uint8_t AK09916_CNTL_3{0x32U};

/* Register Bits */
static constexpr std::uint8_t RESET{0x80U};
static constexpr std::uint8_t I2C_MST_EN{0x20U};
static constexpr std::uint8_t SLEEP{0x40U};
static constexpr std::uint8_t LP_EN{0x20U};
static constexpr std::uint8_t BYPASS_EN{0x02U};
static constexpr std::uint8_t GYR_EN{0x07U};
static constexpr std::uint8_t ACC_EN{0x38U};
static constexpr std::uint8_t FIFO_EN{0x40U};
static constexpr std::uint8_t INT1_ACTL{0x80U};
static constexpr std::uint8_t INT_1_LATCH_EN{0x20U};
static constexpr std::uint8_t ACTL_FSYNC{0x08U};
static constexpr std::uint8_t INT_ANYRD_2CLEAR{0x10U};
static constexpr std::uint8_t FSYNC_INT_MODE_EN{0x06U};
static constexpr std::uint8_t AK09916_16_BIT{0x10U};
static constexpr std::uint8_t AK09916_OVF{0x08U};
static constexpr std::uint8_t AK09916_READ{0x80U};

/* Others */
static constexpr std::uint16_t AK09916_WHO_AM_I_1{0x4809U};
static constexpr std::uint16_t AK09916_WHO_AM_I_2{0x0948U};
static constexpr std::uint8_t  WHO_AM_I_CONTENT{0xEAU};
static constexpr float         ROOM_TEMP_OFFSET{0.0f};
static constexpr float         T_SENSITIVITY{333.87f};
static constexpr float         AK09916_MAG_LSB{0.1495f};

}        // namespace ICM20948

using namespace ICM20948;

/** ===============================================================================================
 *  PUBLIC METHOD DEFINITIONS
 */

icm20948::icm20948(spi_device_handle_t spi, const chip_selector<>& cs) : spi{spi}, cs{cs}
{
}


/* ---------------- FIFO ----------------- */

void icm20948::enable_fifo(bool fifo)
{
    switch_bank(0U);
    std::uint8_t regVal = spi_read<1, USER_CTRL>()[0];

    if(fifo == true)
    {
        regVal |= FIFO_EN;
    }
    else
    {
        regVal &= ~FIFO_EN;
    }

    spi_write<1U>(USER_CTRL, arr1{regVal});
}

void icm20948::set_fifo_mode(ICM20948::Fifo_Mode_Choice mode)
{
    std::uint8_t regVal;
    if(mode != Fifo_Mode_Choice::ICM20948_CONTINUOUS)
    {
        regVal = 0x01;
    }
    else
    {
        regVal = 0x00;
    }

    switch_bank(0U);

    spi_write<1U>(FIFO_MODE, arr1{regVal});
}

void icm20948::start_fifo(ICM20948::Fifo_Type fifo)
{
    fifoType = fifo;

    switch_bank(0U);
    spi_write<1>(FIFO_EN_2, arr1{static_cast<std::uint8_t>(fifoType)});
}

void icm20948::stop_fifo()
{
    switch_bank(0U);
    spi_write<1>(FIFO_EN_2, arr1{0U});
}

void icm20948::reset_fifo()
{
    switch_bank(0U);

    spi_write<1>(FIFO_RST, arr1{0x01U});
    spi_write<1>(FIFO_RST, arr1{0x00U});
}


std::uint16_t icm20948::get_fifo_count()
{
    switch_bank(0U);

    arr2 fifoCount = spi_read<2, FIFO_COUNT>();

    /* TODO: These might be switched - check endianess. */
    uint16_t ret = fifoCount[0U] << 8U | fifoCount[1U];
    return ret;
}

std::uint16_t icm20948::get_number_of_fifo_data_sets()
{
    std::uint16_t numberOfSets = get_fifo_count();

    if((fifoType == Fifo_Type::ICM20948_FIFO_ACC) || (fifoType == Fifo_Type::ICM20948_FIFO_GYR))
    {
        numberOfSets /= 6U;
    }
    else if(fifoType == Fifo_Type::ICM20948_FIFO_ACC_GYR)
    {
        numberOfSets /= 12U;
    }

    return numberOfSets;
}

void icm20948::find_fifo_begin()
{
    switch_bank(0U);

    if((fifoType == Fifo_Type::ICM20948_FIFO_ACC) || (fifoType == Fifo_Type::ICM20948_FIFO_GYR))
    {
        std::uint16_t start = get_fifo_count() % 6U;
        for(std::uint16_t i = 0U; i < start; i++)
        {
            spi_read<1, ICM20948::FIFO_R_W>();
        }
    }
    else if(fifoType == Fifo_Type::ICM20948_FIFO_ACC)
    {
        std::uint16_t start = get_fifo_count() % 12U;
        for(std::uint16_t i = 0U; i < start; i++)
        {
            spi_read<1, ICM20948::FIFO_R_W>();
        }
    }
}


/* ------------ MAGNETOMETER ------------- */
bool icm20948::init_magnetometer()
{
    reset_mag();
    reset();
    sleep(false);

    spi_write(ODR_ALIGN_EN, arr1{0x01U});        // aligns ODR
    vTaskDelay(ICM20948::RESET_DELAY);

    std::uint16_t whoAmI = who_am_i_mag();
    if(!((whoAmI == AK09916_WHO_AM_I_1) || (whoAmI == AK09916_WHO_AM_I_2)))
    {
        return false;
    }
    set_mag_op_mode(Ak09916_Op_Mode::AK09916_CONT_MODE_100HZ);

    return true;
}

std::uint16_t icm20948::who_am_i_mag()
{
    return static_cast<std::uint16_t>(read_ak09916_register16(AK09916_WIA_1));
}

void icm20948::set_mag_op_mode(Ak09916_Op_Mode opMode)
{
    write_ak09916_register8(AK09916_CNTL_2, static_cast<std::uint8_t>(opMode));

    vTaskDelay(ICM20948::RESET_DELAY);

    if(opMode != Ak09916_Op_Mode::AK09916_PWR_DOWN)
    {
        enable_mag_data_read(AK09916_HXL, 0x08);
    }
}

void icm20948::reset_mag()
{
    write_ak09916_register8(AK09916_CNTL_3, 0x01U);

    vTaskDelay(ICM20948::RESET_MAG_DELAY);
}



/** ===============================================================================================
 *  PROTECTED METHOD DEFINITIONS
 */

template<std::size_t N>
std::array<std::uint8_t, N> icm20948::spi_write(std::uint8_t                      reg,
                                                const std::array<std::uint8_t, N> data)
{
    std::array<std::uint8_t, N + 1U> values;
    values[0] = reg;
    std::copy(data.begin(), data.end(), values.begin() + 1U);

    values = spi_write(values);

    std::array<std::uint8_t, N> ret;
    std::copy{values.begin() + 1, values.end(), ret.begin()};

    return ret;
}

template<std::size_t N>
std::array<std::uint8_t, N> icm20948::spi_write(const std::array<std::uint8_t, N> data)
{
    std::array<std::uint8_t, N> receptionData = {0U};

    spi_transaction_t t = {};
    t.length            = data.size();
    t.tx_buffer         = data.begin();
    t.rx_buffer         = &receptionData;
    t.user              = nullptr;

    cs.apply_cfg();
    esp_err_t ret = spi_device_polling_transmit(spi, &t);
    cs.release_cfg();
    assert(ret == ESP_OK);

    return receptionData;
}

template<std::size_t N, std::uint8_t reg>
std::array<std::uint8_t, N> icm20948::spi_read()
{
    /* Create an empty array of N values at compile-time. */
    constexpr std::array<std::uint8_t, N> dummy = []() constexpr
    {
        std::array<std::uint8_t, N> a;
        for(std::size_t i = 0U; i < N; i++)
        {
            a[i] = 0U;
        }
        a[0U] = reg;
        return a;
    }
    ();

    return spi_write(dummy);
}


void icm20948::switch_bank(std::uint8_t newBank)
{
    /* Check current bank, don't switch if already in the right bank. */
    if(newBank == currentBank)
    {
        return;
    }
    currentBank = newBank;

    spi_write(REG_BANK_SEL, arr1{static_cast<std::uint8_t>(currentBank << 4U)});
}



void icm20948::write_ak09916_register8(std::uint8_t reg, std::uint8_t val)
{
    switch_bank(3U);
    spi_write(arr2{I2C_SLV0_ADDR, AK09916_ADDRESS});
    spi_write(arr2{I2C_SLV0_REG, reg});
    spi_write(arr2{I2C_SLV0_DO, val});
}

std::uint8_t icm20948::read_ak09916_register8(std::uint8_t reg)
{
    switch_bank(0U);
    enable_mag_data_read(reg, 0x01U);
    enable_mag_data_read(AK09916_HXL, 0x08U);

    std::uint8_t val = spi_read<1U, EXT_SLV_SENS_DATA_00>()[0U];
    return val;
}

std::int16_t icm20948::read_ak09916_register16(std::uint8_t reg)
{
    switch_bank(0U);
    enable_mag_data_read(reg, 0x02U);
    auto regValue = spi_read<2U, EXT_SLV_SENS_DATA_00>();
    enable_mag_data_read(AK09916_HXL, 0x08U);

    std::int16_t ret = regValue[1U] << 8U | regValue[0U];
    return ret;
}


void icm20948::reset()
{
    switch_bank(0U);
    spi_write(arr2{PWR_MGMT_1, RESET});

    vTaskDelay(ICM20948::RESET_DELAY);
}


vec3<> icm20948::correct_acc_raw_values(vec3<> accRawVal)
{
    return (accRawVal - (accOffsetVal / accRangeFactor)) / accCorrFactor;
}

vec3<> icm20948::correct_gyr_raw_values(vec3<> gyrRawVal)
{
    return gyrRawVal - (gyrOffsetVal / gyrRangeFactor);
}


vec3<> icm20948::read_xyz_val_from_fifo()
{
    using FIFOTriple = std::array<std::uint8_t, 6U>;
    switch_bank(0U);

    uint8_t    reg        = FIFO_R_W | 0x80U;
    FIFOTriple fifoTriple = spi_write(reg, FIFOTriple{0U, 0U, 0U, 0U, 0U, 0U});

    vec3<> xyzResult;
    xyzResult.x = (static_cast<std::uint16_t>((fifoTriple[0U] << 8) + fifoTriple[1U]])) * 1.0;
    xyzResult.y = (static_cast<std::uint16_t>((fifoTriple[2U] << 8) + fifoTriple[3U])) * 1.0;
    xyzResult.z = (static_cast<std::uint16_t>((fifoTriple[4U] << 8) + fifoTriple[5U])) * 1.0;

    return xyzResult;
}

void icm20948::enable_mag_data_read(std::uint8_t reg, std::uint8_t bytes)
{
    switch_bank(3U);

    spi_write(I2C_SLV0_ADDR, arr1{AK09916_ADDRESS | AK09916_READ});        // read AK09916
    spi_write(I2C_SLV0_REG, arr1{reg});        // define AK09916 register to be read
    spi_write(
      I2C_SLV0_CTRL,
      arr1{static_cast<std::uint8_t>(0x80U | bytes)});        // enable read | number of byte

    vTaskDelay(ICM20948::RESET_DELAY);
}


void icm20948::set_clock_to_auto_select()
{
    switch_bank(0U);

    std::uint8_t regVal = spi_read<1U, PWR_MGMT_1>()[0];
    regVal |= 0x01U;

    spi_write<1U>(PWR_MGMT_1, arr1{regVal});

    vTaskDelay(ICM20948::RESET_DELAY);
}


/**
 * ------------------------------------------------------------------------------------------------
 * @}
 */
