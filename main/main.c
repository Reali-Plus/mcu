/**
 * ===============================================================================================
 * @file    main.c
 *
 * @brief   Main file for the Reali+ sleeve project.
 *
 * ------------------------------------------------------------------------------------------------
 * @copyright Copyright (c) 2023 Reali+
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
 * ===============================================================================================
 */


/** ===============================================================================================
 *  INCLUDES
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"


/** ===============================================================================================
 *  DEFINES
 */

#ifdef CONFIG_IDF_TARGET_ESP32S3
#define LCD_HOST SPI2_HOST

#define PIN_NUM_MISO 13 // 37
#define PIN_NUM_MOSI 11 // 35
#define PIN_NUM_CLK 12  // 36
#define PIN_NUM_CS 48

#define PIN_NUM_DC 4
#define PIN_NUM_RST 5
#define PIN_NUM_BCKL 6
#endif


/** ===============================================================================================
 *  FUNCTION DECLARATIONS
 */

void lcd_cmd(spi_device_handle_t spi, uint8_t cmd, bool keep_cs_active);
void app_main(void);


/** ===============================================================================================
 *  FUNCTION DEFINITIONS
 */

/** -----------------------------------------------------------------------------------------------
 * @brief Send a command to the LCD. Uses spi_device_polling_transmit, which waits
 *        until the transfer is complete.
 *
 * Since command transactions are usually small, they are handled in polling
 * mode for higher speed. The overhead of interrupt transactions is more than
 * just waiting for the transaction to complete.
 */
void lcd_cmd(spi_device_handle_t spi, const uint8_t cmd, bool keep_cs_active)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t)); // Zero out the transaction
    t.length = 8;             // Command is 8 bits
    t.tx_buffer = &cmd;       // The data is the cmd itself
    t.user = (void *)0;       // D/C needs to be set to 0
    if (keep_cs_active)
    {
        t.flags = SPI_TRANS_CS_KEEP_ACTIVE; // Keep CS active after data transfer
    }
    ret = spi_device_polling_transmit(spi, &t); // Transmit!
    assert(ret == ESP_OK);                      // Should have had no issues.
}

void app_main(void)
{
    printf("-----------------------------\n");
    printf("Bon matin\n");

    esp_err_t ret;

    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32};

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1 * 1000 * 1000, // Clock out at 1 MHz
        .mode = 0,                         // SPI mode 0
        .spics_io_num = PIN_NUM_CS,        // CS pin
        .queue_size = 7,                   // We want to be able to queue 7 transactions at a time
        .pre_cb = NULL,                    // Specify pre-transfer callback to handle D/C line
    };

    printf("Initializing\n");
    // Initialize the SPI bus
    ret = spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    // Attach the LCD to the SPI bus
    spi_device_handle_t spi;
    ret = spi_bus_add_device(LCD_HOST, &devcfg, &spi);
    printf("Added device\n");
    ESP_ERROR_CHECK(ret);

    printf("0xCC\n");
    lcd_cmd(spi, 0xCC, false);

    printf("Joyeux dodo\n");
    while (1)
    {
        lcd_cmd(spi, 0xCC, false);
    }
}


/**
 * ------------------------------------------------------------------------------------------------
 */
