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
#ifndef ICM20948_H
#define ICM20948_H


/** ===============================================================================================
 *  INCLUDES
 */
#include "icm20948_enum.h"

#include "chip_selector.h"
#include "vec3.h"

#include "driver/spi_master.h"



/** ===============================================================================================
 *  CLASS DEFINITION
 */
class icm20948
{
public:
    chip_selector<>     cs;
    spi_device_handle_t spi;

    icm20948()                 = delete;
    icm20948(const icm20948&)  = delete;
    icm20948(const icm20948&&) = delete;
    icm20948(spi_device_handle_t spi, const chip_selector<>& cs);


    uint8_t whoAmI();


    /* ----------- BASIC SETTINGS ------------ */
    void auto_offsets();
    void set_acc_offsets(vec3<> min, vec3<> max);
    void set_gyr_offsets(vec3<> offset);

    void enable_acc(bool enAcc);
    void set_acc_range(ICM20948::Acc_Range accRange);
    void set_acc_dlpf(ICM20948::Dlpf dlpf);
    void set_acc_sample_rate_divider(std::uint16_t accSplRateDiv);

    void enable_gyr(bool enGyr);
    void set_gyr_range(ICM20948::Gyro_Range gyroRange);
    void set_gyr_dlpf(ICM20948::Dlpf dlpf);
    void set_gyr_sample_rate_divider(uint8_t gyrSplRateDiv);

    void set_temp_dlpf(ICM20948::Dlpf dlpf);


    /* ------------ X Y Z RESULTS ------------ */
    void read_sensor();

    vec3<> get_g_values();
    vec3<> get_g_values_from_fifo();
    vec3<> get_acc_raw_values();
    vec3<> get_acc_raw_values_from_fifo();
    vec3<> get_corrected_acc_raw_values_from_fifo();
    vec3<> get_corrected_acc_raw_values();
    float  get_resultant_g(vec3<> gVal);

    vec3<> get_gyr_raw_values();
    vec3<> get_corrected_gyr_raw_values();
    vec3<> get_gyr_values();
    vec3<> get_gyr_values_from_fifo();
    vec3<> get_mag_values();
    float  get_temperature();


    /* ------- ANGLES AND ORIENTATION -------- */
    vec3<>                get_angles();
    ICM20948::Orientation get_orientation();
    float                 get_pitch();
    float                 get_roll();


    /* -------- Power, Sleep, Standby -------- */
    void enable_cycle(ICM20948::Cycle cycle);
    void enable_low_power(bool enLP);
    void set_gyr_average_in_cycle_mode(ICM20948::Gyro_Avg_Low_Pwr avg);
    void set_acc_average_in_cycle_mode(ICM20948::Acc_Avg_Low_Pwr avg);
    void sleep(bool sleep);


    /* ---------------- FIFO ----------------- */
    void enable_fifo(bool fifo);
    void set_fifo_mode(ICM20948::Fifo_Mode_Choice mode);
    void start_fifo(ICM20948::Fifo_Type fifo);
    void stop_fifo();
    void reset_fifo();

    std::int16_t get_fifo_count();
    std::int16_t get_number_of_fifo_data_sets();
    void         find_fifo_begin();

    /* ------------ MAGNETOMETER ------------- */
    bool          init_magnetometer();
    std::uint16_t who_am_i_mag();
    void          set_mag_op_mode(ICM20948::Ak09916_Op_Mode opMode);
    void          reset_mag();


protected:
    void reset();

    vec3<> correct_acc_raw_values(vec3<> accRawVal);
    vec3<> correct_gyr_raw_values(vec3<> gyrRawVal);

    vec3<> read_xyz_val_from_fifo();
    void   enable_mag_data_read(std::uint8_t reg, std::uint8_t bytes);

    void         set_clock_to_auto_select();
    void         switch_bank(std::uint8_t newBank);
    void         write_register8(std::uint8_t bank, std::uint8_t reg, std::uint8_t val);
    void         write_register16(std::uint8_t bank, std::uint8_t reg, std::int16_t val);
    std::uint8_t read_register8(std::uint8_t bank, std::uint8_t reg);
    std::int16_t read_register16(std::uint8_t bank, std::uint8_t reg);
    void         read_all_data(std::uint8_t* data);

    void         write_ak09916_register8(std::uint8_t reg, std::uint8_t val);
    std::uint8_t read_ak09916_register8(std::uint8_t reg);
    std::int16_t read_ak09916_register16(std::uint8_t reg);
};


#endif        // ICM20948_H

/**
 * ------------------------------------------------------------------------------------------------
 * @}
 */
