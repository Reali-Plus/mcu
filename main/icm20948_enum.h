/**
 * ===============================================================================================
 * @addtogroup icm20948
 * @{
 * ------------------------------------------------------------------------------------------------
 * @file    icm20948_enum.h
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
 * @warning This library is used in a specific project, which only uses SPI.
 *          Only the SPI functionalities have been ported from the original library.\n
 *          In addition, the SPI of this project is special, using combination of pins to represent
 *          the Chip Select of multiple ICM20948s. This class has such been made to be instanciated
 *          and uses truth tables to communicate with the selected IMU.
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
#ifndef ICM20948_ENUM_H
#define ICM20948_ENUM_H


/** ===============================================================================================
 *  INCLUDES
 */
#include <cstdint>


/** ===============================================================================================
 *  ENUMS
 */
namespace ICM20948
{
enum class Cycle : std::uint8_t
{
    ICM20948_NO_CYCLE              = 0x00,
    ICM20948_GYR_CYCLE             = 0x10,
    ICM20948_ACC_CYCLE             = 0x20,
    ICM20948_ACC_GYR_CYCLE         = 0x30,
    ICM20948_ACC_GYR_I2C_MST_CYCLE = 0x70
};

enum class Int_Pin_Pol : std::uint8_t
{
    ICM20948_ACT_HIGH,
    ICM20948_ACT_LOW
};

enum class Int_Type : std::uint8_t
{
    ICM20948_FSYNC_INT      = 0x01,
    ICM20948_WOM_INT        = 0x02,
    ICM20948_DMP_INT        = 0x04,
    ICM20948_DATA_READY_INT = 0x08,
    ICM20948_FIFO_OVF_INT   = 0x10,
    ICM20948_FIFO_WM_INT    = 0x20
};

enum class Fifo_Type : std::uint8_t
{
    ICM20948_FIFO_ACC     = 0x10,
    ICM20948_FIFO_GYR     = 0x0E,
    ICM20948_FIFO_ACC_GYR = 0x1E
};

enum class Fifo_Mode_Choice : std::uint8_t
{
    ICM20948_CONTINUOUS,
    ICM20948_STOP_WHEN_FULL
};

enum class Gyro_Range : std::uint8_t
{
    ICM20948_GYRO_RANGE_250,
    ICM20948_GYRO_RANGE_500,
    ICM20948_GYRO_RANGE_1000,
    ICM20948_GYRO_RANGE_2000
};

enum class Dlpf : std::uint8_t
{
    ICM20948_DLPF_0,
    ICM20948_DLPF_1,
    ICM20948_DLPF_2,
    ICM20948_DLPF_3,
    ICM20948_DLPF_4,
    ICM20948_DLPF_5,
    ICM20948_DLPF_6,
    ICM20948_DLPF_7,
    ICM20948_DLPF_OFF
};

enum class Gyro_Avg_Low_Pwr : std::uint8_t
{
    ICM20948_GYR_AVG_1,
    ICM20948_GYR_AVG_2,
    ICM20948_GYR_AVG_4,
    ICM20948_GYR_AVG_8,
    ICM20948_GYR_AVG_16,
    ICM20948_GYR_AVG_32,
    ICM20948_GYR_AVG_64,
    ICM20948_GYR_AVG_128
};

enum class Acc_Range : std::uint8_t
{
    ICM20948_ACC_RANGE_2G,
    ICM20948_ACC_RANGE_4G,
    ICM20948_ACC_RANGE_8G,
    ICM20948_ACC_RANGE_16G
};

enum class Acc_Avg_Low_Pwr : std::uint8_t
{
    ICM20948_ACC_AVG_4,
    ICM20948_ACC_AVG_8,
    ICM20948_ACC_AVG_16,
    ICM20948_ACC_AVG_32
};

enum class Wom_Comp : std::uint8_t
{
    ICM20948_WOM_COMP_DISABLE,
    ICM20948_WOM_COMP_ENABLE
};

enum class Ak09916_Op_Mode : std::uint8_t
{
    AK09916_PWR_DOWN        = 0x00,
    AK09916_TRIGGER_MODE    = 0x01,
    AK09916_CONT_MODE_10HZ  = 0x02,
    AK09916_CONT_MODE_20HZ  = 0x04,
    AK09916_CONT_MODE_50HZ  = 0x06,
    AK09916_CONT_MODE_100HZ = 0x08
};

enum class Orientation : std::uint8_t
{
    ICM20948_FLAT,
    ICM20948_FLAT_1,
    ICM20948_XY,
    ICM20948_XY_1,
    ICM20948_YX,
    ICM20948_YX_1
};


}        // namespace ICM20948

#endif        // ICM20948_ENUM_H

/**
 * ------------------------------------------------------------------------------------------------
 * @}
 */