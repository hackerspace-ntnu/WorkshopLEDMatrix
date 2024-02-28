/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
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

typedef struct {
    led_strip_t base;
    spi_host_device_t spi_host;
    spi_device_handle_t spi_device;
    uint32_t strip_len;
    uint8_t bytes_per_pixel;
    uint8_t pixel_buf[];
} led_strip_spi_obj;

//forward declaration
esp_err_t led_strip_new_spi_device(const led_strip_config_t *led_config, const led_strip_spi_config_t *spi_config, led_strip_handle_t *ret_strip);


#ifdef USE_SPI

led_strip_handle_t configure_led(void) {
    // LED strip general initialization, according to your led board design
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_STRIP_BLINK_GPIO,   // The GPIO that connected to the LED strip's data line
        .max_leds = LED_STRIP_LED_NUMBERS,        // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812,            // LED strip model
        .flags.invert_out = false,                // whether to invert the output signal
    };

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
    return led_strip;
}
#else 

led_strip_handle_t configure_led(void) {
    // LED strip general initialization, according to your led board design
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_STRIP_BLINK_GPIO,   // The GPIO that connected to the LED strip's data line
        .max_leds = LED_STRIP_LED_NUMBERS,        // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812,            // LED strip model
        .flags.invert_out = false,                // whether to invert the output signal
    };

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
    return led_strip;
}

#endif // end if USE_SPI

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

// FROM led_strip_spi_dev.c

// please make sure to zero-initialize the buf before calling this function
static void __led_strip_spi_bit(uint8_t data, uint8_t *buf)
{
    // Each color of 1 bit is represented by 3 bits of SPI, low_level:100 ,high_level:110
    // So a color byte occupies 3 bytes of SPI.
    *(buf + 2) |= data & BIT(0) ? BIT(2) | BIT(1) : BIT(2);
    *(buf + 2) |= data & BIT(1) ? BIT(5) | BIT(4) : BIT(5);
    *(buf + 2) |= data & BIT(2) ? BIT(7) : 0x00;
    *(buf + 1) |= BIT(0);
    *(buf + 1) |= data & BIT(3) ? BIT(3) | BIT(2) : BIT(3);
    *(buf + 1) |= data & BIT(4) ? BIT(6) | BIT(5) : BIT(6);
    *(buf + 0) |= data & BIT(5) ? BIT(1) | BIT(0) : BIT(1);
    *(buf + 0) |= data & BIT(6) ? BIT(4) | BIT(3) : BIT(4);
    *(buf + 0) |= data & BIT(7) ? BIT(7) | BIT(6) : BIT(7);
}

static esp_err_t led_strip_spi_set_pixel(led_strip_t *strip, uint32_t index, uint32_t red, uint32_t green, uint32_t blue)
{
    led_strip_spi_obj *spi_strip = __containerof(strip, led_strip_spi_obj, base);
    ESP_RETURN_ON_FALSE(index < spi_strip->strip_len, ESP_ERR_INVALID_ARG, TAG, "index out of maximum number of LEDs");
    // LED_PIXEL_FORMAT_GRB takes 72bits(9bytes)
    uint32_t start = index * spi_strip->bytes_per_pixel * SPI_BYTES_PER_COLOR_BYTE;
    memset(spi_strip->pixel_buf + start, 0, spi_strip->bytes_per_pixel * SPI_BYTES_PER_COLOR_BYTE);
    __led_strip_spi_bit(green, &spi_strip->pixel_buf[start]);
    __led_strip_spi_bit(red, &spi_strip->pixel_buf[start + SPI_BYTES_PER_COLOR_BYTE]);
    __led_strip_spi_bit(blue, &spi_strip->pixel_buf[start + SPI_BYTES_PER_COLOR_BYTE * 2]);
    if (spi_strip->bytes_per_pixel > 3) {
        __led_strip_spi_bit(0, &spi_strip->pixel_buf[start + SPI_BYTES_PER_COLOR_BYTE * 3]);
    }
    return ESP_OK;
}

static esp_err_t led_strip_spi_set_pixel_rgbw(led_strip_t *strip, uint32_t index, uint32_t red, uint32_t green, uint32_t blue, uint32_t white)
{
    led_strip_spi_obj *spi_strip = __containerof(strip, led_strip_spi_obj, base);
    ESP_RETURN_ON_FALSE(index < spi_strip->strip_len, ESP_ERR_INVALID_ARG, TAG, "index out of maximum number of LEDs");
    ESP_RETURN_ON_FALSE(spi_strip->bytes_per_pixel == 4, ESP_ERR_INVALID_ARG, TAG, "wrong LED pixel format, expected 4 bytes per pixel");
    // LED_PIXEL_FORMAT_GRBW takes 96bits(12bytes)
    uint32_t start = index * spi_strip->bytes_per_pixel * SPI_BYTES_PER_COLOR_BYTE;
    // SK6812 component order is GRBW
    memset(spi_strip->pixel_buf + start, 0, spi_strip->bytes_per_pixel * SPI_BYTES_PER_COLOR_BYTE);
    __led_strip_spi_bit(green, &spi_strip->pixel_buf[start]);
    __led_strip_spi_bit(red, &spi_strip->pixel_buf[start + SPI_BYTES_PER_COLOR_BYTE]);
    __led_strip_spi_bit(blue, &spi_strip->pixel_buf[start + SPI_BYTES_PER_COLOR_BYTE * 2]);
    __led_strip_spi_bit(white, &spi_strip->pixel_buf[start + SPI_BYTES_PER_COLOR_BYTE * 3]);

    return ESP_OK;
}

static esp_err_t led_strip_spi_refresh(led_strip_t *strip)
{
    led_strip_spi_obj *spi_strip = __containerof(strip, led_strip_spi_obj, base);
    spi_transaction_t tx_conf;
    memset(&tx_conf, 0, sizeof(tx_conf));

    tx_conf.length = spi_strip->strip_len * spi_strip->bytes_per_pixel * SPI_BITS_PER_COLOR_BYTE;
    tx_conf.tx_buffer = spi_strip->pixel_buf;
    tx_conf.rx_buffer = NULL;
    ESP_RETURN_ON_ERROR(spi_device_transmit(spi_strip->spi_device, &tx_conf), TAG, "transmit pixels by SPI failed");

    return ESP_OK;
}

static esp_err_t led_strip_spi_clear(led_strip_t *strip)
{
    led_strip_spi_obj *spi_strip = __containerof(strip, led_strip_spi_obj, base);
    //Write zero to turn off all leds
    memset(spi_strip->pixel_buf, 0, spi_strip->strip_len * spi_strip->bytes_per_pixel * SPI_BYTES_PER_COLOR_BYTE);
    uint8_t *buf = spi_strip->pixel_buf;
    for (int index = 0; index < spi_strip->strip_len * spi_strip->bytes_per_pixel; index++) {
        __led_strip_spi_bit(0, buf);
        buf += SPI_BYTES_PER_COLOR_BYTE;
    }

    return led_strip_spi_refresh(strip);
}

static esp_err_t led_strip_spi_del(led_strip_t *strip)
{
    led_strip_spi_obj *spi_strip = __containerof(strip, led_strip_spi_obj, base);

    ESP_RETURN_ON_ERROR(spi_bus_remove_device(spi_strip->spi_device), TAG, "delete spi device failed");
    ESP_RETURN_ON_ERROR(spi_bus_free(spi_strip->spi_host), TAG, "free spi bus failed");

    free(spi_strip);
    return ESP_OK;
}


esp_err_t led_strip_new_spi_device(const led_strip_config_t *led_config, const led_strip_spi_config_t *spi_config, led_strip_handle_t *ret_strip)
{
    led_strip_spi_obj *spi_strip = NULL;
    esp_err_t ret = ESP_OK;
    ESP_GOTO_ON_FALSE(led_config && spi_config && ret_strip, ESP_ERR_INVALID_ARG, err, TAG, "invalid argument");
    ESP_GOTO_ON_FALSE(led_config->led_pixel_format < LED_PIXEL_FORMAT_INVALID, ESP_ERR_INVALID_ARG, err, TAG, "invalid led_pixel_format");
    uint8_t bytes_per_pixel = 3;
    if (led_config->led_pixel_format == LED_PIXEL_FORMAT_GRBW) {
        bytes_per_pixel = 4;
    } else if (led_config->led_pixel_format == LED_PIXEL_FORMAT_GRB) {
        bytes_per_pixel = 3;
    } else {
        assert(false);
    }
    uint32_t mem_caps = MALLOC_CAP_DEFAULT;
    if (spi_config->flags.with_dma) {
        // DMA buffer must be placed in internal SRAM
        mem_caps |= MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA;
    }
    spi_strip = heap_caps_calloc(1, sizeof(led_strip_spi_obj) + led_config->max_leds * bytes_per_pixel * SPI_BYTES_PER_COLOR_BYTE, mem_caps);

    ESP_GOTO_ON_FALSE(spi_strip, ESP_ERR_NO_MEM, err, TAG, "no mem for spi strip");

    spi_strip->spi_host = spi_config->spi_bus;
    // for backward compatibility, if the user does not set the clk_src, use the default value
    // spi_clock_source_t clk_src = SPI_CLK_SRC_DEFAULT; // not used 
    // spi_clock_source_t clk_src = SOC_MOD_CLK_APB; // not used
    // if (spi_config->clk_src) {
    //     clk_src = spi_config->clk_src;
    // }

    spi_bus_config_t spi_bus_cfg = {
        .mosi_io_num = led_config->strip_gpio_num,
        //Only use MOSI to generate the signal, set -1 when other pins are not used.
        .miso_io_num = -1,
        .sclk_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = led_config->max_leds * bytes_per_pixel * SPI_BYTES_PER_COLOR_BYTE,
    };
    ESP_GOTO_ON_ERROR(spi_bus_initialize(spi_strip->spi_host, &spi_bus_cfg, spi_config->flags.with_dma ? SPI_DMA_CH_AUTO : SPI_DMA_DISABLED), err, TAG, "create SPI bus failed");

    if (led_config->flags.invert_out == true) {
        esp_rom_gpio_connect_out_signal(led_config->strip_gpio_num, spi_periph_signal[spi_strip->spi_host].spid_out, true, false);
    }

    spi_device_interface_config_t spi_dev_cfg = {
        .command_bits = 0,
        // .clock_source = clk_src, // if clock_source is not set, SPI_CLK_SRC_DEFAULT by default. 
        .address_bits = 0,
        .dummy_bits = 0,
        .clock_speed_hz = LED_STRIP_SPI_DEFAULT_RESOLUTION,
        .mode = 0,
        //set -1 when CS is not used
        .spics_io_num = -1,
        .queue_size = LED_STRIP_SPI_DEFAULT_TRANS_QUEUE_SIZE,
    };

    ESP_GOTO_ON_ERROR(spi_bus_add_device(spi_strip->spi_host, &spi_dev_cfg, &spi_strip->spi_device), err, TAG, "Failed to add spi device");

    int clock_resolution_khz = 0;
    spi_device_get_actual_freq(spi_strip->spi_device, &clock_resolution_khz);
    // TODO: ideally we should decide the SPI_BYTES_PER_COLOR_BYTE by the real clock resolution
    // But now, let's fixed the resolution, the downside is, we don't support a clock source whose frequency is not multiple of LED_STRIP_SPI_DEFAULT_RESOLUTION
    ESP_GOTO_ON_FALSE(clock_resolution_khz == LED_STRIP_SPI_DEFAULT_RESOLUTION / 1000, ESP_ERR_NOT_SUPPORTED, err,
                      TAG, "unsupported clock resolution:%dKHz", clock_resolution_khz);

    spi_strip->bytes_per_pixel = bytes_per_pixel;
    spi_strip->strip_len = led_config->max_leds;
    spi_strip->base.set_pixel = led_strip_spi_set_pixel;
    spi_strip->base.set_pixel_rgbw = led_strip_spi_set_pixel_rgbw;
    spi_strip->base.refresh = led_strip_spi_refresh;
    spi_strip->base.clear = led_strip_spi_clear;
    spi_strip->base.del = led_strip_spi_del;

    *ret_strip = &spi_strip->base;
    return ESP_OK;
err:
    if (spi_strip) {
        if (spi_strip->spi_device) {
            spi_bus_remove_device(spi_strip->spi_device);
        }
        if (spi_strip->spi_host) {
            spi_bus_free(spi_strip->spi_host);
        }
        free(spi_strip);
    }
    return ret;
}

