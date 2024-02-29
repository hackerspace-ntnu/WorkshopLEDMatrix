#ifndef CONFIG_LED_MATRIX_H
#define CONFIG_LED_MATRIX_H

#define USE_SPI 1

// GPIO assignment
#define LED_STRIP_BLINK_GPIO  2
// Numbers of the LED in the strip
#define LED_STRIP_LED_NUMBERS 24
// 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define LED_STRIP_RMT_RES_HZ  (10 * 1000 * 1000)


#endif