#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "esp_log.h"
#include "esp_err.h"
#include "tcp_server.h"
#include "tcp_client.h"
#include "led_matrix_config.h"


static const char *TAG = "LED Matrix";

led_strip_handle_t configure_led(void) {

    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_STRIP_GPIO,
        .max_leds = LED_STRIP_LED_NUMBER,        
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, 
        .led_model = LED_MODEL_WS2812,           
        .flags.invert_out = false,                
    };

    #if LED_STRIP_USE_SPI
        led_strip_spi_config_t spi_config = {
            .clk_src = SOC_MOD_CLK_APB, // different clock source can lead to different power consumption, 
                                        // from clk_tree_defs SPI_CLK_SRC_DEFAULT is set to SOC_MOD_CLK_APB 
            .flags.with_dma = true,         // Using DMA can improve performance and help drive more LEDs
            .spi_bus = SPI2_HOST,           // SPI bus ID
        };

        led_strip_handle_t led_strip;
        ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &led_strip));
        ESP_LOGI(TAG, "Created LED strip object with SPI backend");
        
    #else
       
        led_strip_rmt_config_t rmt_config = {
            #if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
                    .rmt_channel = 0,
            #else
                .clk_src = RMT_CLK_SRC_DEFAULT,        // different clock source can lead to different power consumption
                .resolution_hz = CONFIG_ESP_LED_STRIP_RMT_RES_HZ, // RMT counter clock frequency
                .flags.with_dma = false,               // DMA feature is available on ESP target like ESP32-S3
            #endif
        };

        led_strip_handle_t led_strip;
        ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
        ESP_LOGI(TAG, "Created LED strip object with RMT backend");
    #endif
    
    return led_strip;
}

void app_main(void)  {

    start_server();
    // start_client();
    

    // led_strip_handle_t led_strip = configure_led();
    // bool led_on_off = false;

    // ESP_LOGI(TAG, "Start blinking LED strip");
    // while (1) {
    //     if (led_on_off) {
    //         // uint8_t pattern[CONFIG_ESP_LED_STRIP_LED_NUMBER] = {
    //         //     0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,
    //         // };
    //         // Map the pattern to the LED strip
    //         for (int i = 0; i < CONFIG_ESP_LED_STRIP_LED_NUMBER; i++) {
    //             ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, i, 255 : 0, pattern[i] ? 255 : 0, pattern[i] ? 255 : 0));
    //         }

    //         /* Refresh the strip to send data */
    //         ESP_ERROR_CHECK(led_strip_refresh(led_strip));
    //         ESP_LOGI(TAG, "LED ON!");
    //     } else {
    //         /* Set all LED off to clear all pixels */
    //         ESP_ERROR_CHECK(led_strip_clear(led_strip));
    //         ESP_LOGI(TAG, "LED OFF!");
    //     }
    //     led_on_off = !led_on_off;
    //     vTaskDelay(pdMS_TO_TICKS(500));
    // }
}




