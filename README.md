# WorkshopLEDMatrix

## Project Description
The goal of this project is to make a LED matrix/sign for displaying messages in the window. It's controlled by a Raspberry Pi and an ESP32.


### Implementation:
The ESP32 will be responsible for sending data to the LED matrix using RMT or SPI depending on which one works best with many LEDs. It will also act as a TCP-server and receive the text you want to display in the window from the RPi. Connect to the RPi via ssh and run `python3 send_text.py "xxx"` to display "xxx" on the LED Matrix. 


## Dimensions 
The plan is to make it 12 LEDs in height and about 100 LEDs in length, which roughly translate to 20 cm and 160 cm. 


## Code 
Python running on the RPi and C using the Espress-if build system on the ESP32.


## TODO
* Acquire a suitable power supply for all the LEDs (it's a lot!)
* More LED-strips