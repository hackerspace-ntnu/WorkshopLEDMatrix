/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
// #include "led_strip_spi.h"
#include "esp_log.h"
#include "esp_err.h"
#include "tcp_server.h"
#include "config.h"
// #include <stdlib.h>
// #include <string.h>
#include <sys/cdefs.h>
#include "esp_check.h"
#include "esp_rom_gpio.h"
#include "soc/spi_periph.h"
#include "led_strip_interface.h"
#include "hal/spi_hal.h"
#include "soc/clk_tree_defs.h" 

static const char *TAG = "LED Matrix";


led_strip_handle_t configure_led(void) {
    // LED strip general initialization, according to your led board design
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_STRIP_BLINK_GPIO,   // The GPIO that connected to the LED strip's data line
        .max_leds = LED_STRIP_LED_NUMBERS,        // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812,            // LED strip model
        .flags.invert_out = false,                // whether to invert the output signal
    };

    #ifdef USE_SPI
        // LED strip backend configuration: SPI
        led_strip_spi_config_t spi_config = {
            //  .clk_src = SPI_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
            .clk_src = SOC_MOD_CLK_APB, // different clock source can lead to different power consumption, from clk_tree_defs SPI_CLK_SRC_DEFAULT is set to SOC_MOD_CLK_APB 
            .flags.with_dma = true,         // Using DMA can improve performance and help drive more LEDs
            .spi_bus = SPI2_HOST,           // SPI bus ID
        };

        // LED Strip object handle
        led_strip_handle_t led_strip;
        ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &led_strip));
        ESP_LOGI(TAG, "Created LED strip object with SPI backend");
        
    #else
        // LED strip backend configuration: RMT
        led_strip_rmt_config_t rmt_config = {
            #if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
                    .rmt_channel = 0,
            #else
                .clk_src = RMT_CLK_SRC_DEFAULT,        // different clock source can lead to different power consumption
                .resolution_hz = LED_STRIP_RMT_RES_HZ, // RMT counter clock frequency
                .flags.with_dma = false,               // DMA feature is available on ESP target like ESP32-S3
            #endif
        };

        // LED Strip object handle
        led_strip_handle_t led_strip;
        ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
        ESP_LOGI(TAG, "Created LED strip object with RMT backend");
    #endif
    
    return led_strip;
}

void app_main(void)  {
    // Code for TCP server
    /*
         ESP_ERROR_CHECK(nvs_flash_init());
        esp_netif_init();
        esp_event_loop_create_default();

        wifi_init_sta();
        start_server();
    */

    led_strip_handle_t led_strip = configure_led();
    bool led_on_off = false;

    ESP_LOGI(TAG, "Start blinking LED strip");
    while (1) {
        if (led_on_off) {
            /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
            for (int i = 0; i < LED_STRIP_LED_NUMBERS; i++) {
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, i, 5, 5, 5));
            }
            /* Refresh the strip to send data */
            ESP_ERROR_CHECK(led_strip_refresh(led_strip));
            ESP_LOGI(TAG, "LED ON!");
        } else {
            /* Set all LED off to clear all pixels */
            ESP_ERROR_CHECK(led_strip_clear(led_strip));
            ESP_LOGI(TAG, "LED OFF!");
        }

        led_on_off = !led_on_off;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
