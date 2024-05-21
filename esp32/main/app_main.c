#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "esp_log.h"
#include "esp_err.h"
#include "tcp_server.h"
#include "tcp_client.h"
#include "led_matrix_config.h"


static const char *TAG = "LED Matrix";

led_strip_handle_t setup_led_strip(bool use_spi, uint32_t number_of_leds, int gpio_pin) {

    led_strip_config_t led_strip_config = {
        .strip_gpio_num = gpio_pin,
        .max_leds = number_of_leds,        
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, 
        .led_model = LED_MODEL_WS2812,           
        .flags.invert_out = false,                
    };

    led_strip_handle_t led_strip;

    if (use_spi) {
        led_strip_spi_config_t spi_config = {
            .clk_src = SPI_CLK_APB,         // different clock source can lead to different power consumption
            .flags.with_dma = true,         // Using DMA can improve performance and help drive more LEDs
            .spi_bus = SPI2_HOST,           // SPI bus ID
        };
        
        ESP_ERROR_CHECK(led_strip_new_spi_device(&led_strip_config, &spi_config, &led_strip));
        ESP_LOGI(TAG, "Created LED strip object using SPI");
        
    } else {
       
        led_strip_rmt_config_t rmt_config = {
            #if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
                    .rmt_channel = 0,
            #else
                .clk_src = RMT_CLK_SRC_DEFAULT,        // different clock source can lead to different power consumption
                .resolution_hz = 10000000,             // RMT counter clock frequency:  10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
                .flags.with_dma = false,               // DMA feature is available on ESP target like ESP32-S3
            #endif
        };

        ESP_ERROR_CHECK(led_strip_new_rmt_device(&led_strip_config, &rmt_config, &led_strip));
        ESP_LOGI(TAG, "Created LED strip object using RMT");
    }
    
    return led_strip;
}

void app_main(void)  {

    start_server();
    
    bool use_spi = true;
    uint32_t number_of_leds = 40;
    int gpio_pin = 13;

    led_strip_handle_t led_strip = setup_led_strip(use_spi, number_of_leds, gpio_pin);
    bool is_led_on = false;

    while (1) {
        if (is_led_on) {
            for (int i = 0; i < number_of_leds; i++) {
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, i, 255, 255, 255));
            }

            /* Refresh the strip to send data */
            ESP_ERROR_CHECK(led_strip_refresh(led_strip));
            ESP_LOGI(TAG, "LED ON!");
        } else {
            /* Set all LED off to clear all pixels */
            ESP_ERROR_CHECK(led_strip_clear(led_strip));
            ESP_LOGI(TAG, "LED OFF!");
        }
        is_led_on = !is_led_on;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

