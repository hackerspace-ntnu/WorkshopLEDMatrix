# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/joakim/esp/esp-idf/components/bootloader/subproject"
  "/home/joakim/code/Joakim-droid/WorkshopLEDMatrix/esp32_tcpClient/managed_components/espressif__led_strip/examples/led_strip_spi_ws2812/build/bootloader"
  "/home/joakim/code/Joakim-droid/WorkshopLEDMatrix/esp32_tcpClient/managed_components/espressif__led_strip/examples/led_strip_spi_ws2812/build/bootloader-prefix"
  "/home/joakim/code/Joakim-droid/WorkshopLEDMatrix/esp32_tcpClient/managed_components/espressif__led_strip/examples/led_strip_spi_ws2812/build/bootloader-prefix/tmp"
  "/home/joakim/code/Joakim-droid/WorkshopLEDMatrix/esp32_tcpClient/managed_components/espressif__led_strip/examples/led_strip_spi_ws2812/build/bootloader-prefix/src/bootloader-stamp"
  "/home/joakim/code/Joakim-droid/WorkshopLEDMatrix/esp32_tcpClient/managed_components/espressif__led_strip/examples/led_strip_spi_ws2812/build/bootloader-prefix/src"
  "/home/joakim/code/Joakim-droid/WorkshopLEDMatrix/esp32_tcpClient/managed_components/espressif__led_strip/examples/led_strip_spi_ws2812/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/joakim/code/Joakim-droid/WorkshopLEDMatrix/esp32_tcpClient/managed_components/espressif__led_strip/examples/led_strip_spi_ws2812/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/joakim/code/Joakim-droid/WorkshopLEDMatrix/esp32_tcpClient/managed_components/espressif__led_strip/examples/led_strip_spi_ws2812/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
