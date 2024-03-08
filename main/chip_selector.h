/**
 * ===============================================================================================
 * @addtogroup chip_selector
 * @{
 * ------------------------------------------------------------------------------------------------
 * @file    chip_selector.h
 * @author  Pascal-Emmanuel Lachance
 * @p       <a href="https://www.github.com/Raesangur">Raesangur</a>
 * @p       <a href="https://www.raesangur.com/">https://www.raesangur.com/</a>
 *
 * @brief Class containing pin# information for a number of chip select pins, as well as utility
 *        around truth tables for these chip select pins.
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
 * ------------------------------------------------------------------------------------------------
 * File History:
 * @version 0.1
 * 2024-03-07- Raesangur
 *      - Initial implementation
 * ===============================================================================================
 */
#ifndef CHIP_SELECTOR_H
#define CHIP_SELECTOR_H


/** ===============================================================================================
 *  INCLUDES
 */
#include "driver/gpio.h"


#include <array>
#include <cstddef>
#include <cstdint>



/** ===============================================================================================
 *  CLASS DEFINITION
 */
template<std::size_t cs_num = 5, bool default_state = true>
class chip_selector
{
    template<typename T>
    using pins_t = std::array<T, cs_num>;
    using io_t   = gpio_num_t;

public:
    pins_t<io_t> pins;
    pins_t<bool> cfg;

    chip_selector() = delete;
    chip_selector(pins_t<io_t> pins, pins_t<bool> cfg) : pins{pins}, cfg{cfg};
    chip_selector(const chip_selector&) = default;

    void apply_cfg();
    void release_cfg();
};

template<std::size_t cs, bool ds>
void chip_selector<cs, ds>::apply_cfg()
{
    for(std::size_t i = 0; i < cs_num; i++)
    {
        gpio_set_level(pins[i], static_cast<std::uint32_t>(cfg[i]));
    }
}

template<std::size_t cs, bool ds>
void chip_selector<cs, ds>::release_cfg()
{
    for(std::size_t i = 0; i < cs_num; i++)
    {
        gpio_set_level(pins[i], static_cast<std::uint32_t>(default_state));
    }
}


#endif        // CHIP_SELECTOR_H

/**
 * ------------------------------------------------------------------------------------------------
 * @}
 */
