# Touch7834 driver for the ADS 7834 resistive touch screen driver

Construct it with the chip select pin number, and perhaps a SPI clock.
(The chip is specified up to 2 MHz, lower values may give better precision)

Optionally provide calibration ranges for X and Y axes.

The orientation is correct for setOrientation(3) for the ILI9341 based 
screens.

Call touch.read(&x, &y) to get the position being touched. If the screen is 
not being touched, the function will return false.

MIT license. Copyright 2017 Jon Watte (jwatte@gmail.com)


    #include "Touch7834.h"
    
    Touch7834 touch(TOUCH_CHIP_SELECT_PIN);
    
    void setup() {
        touch.begin();
    }
    
    void loop() {
        int16_t x, y;
        if (touch.read(&x, &y)) {
            do_something_with_position(x, y);
        }
    }
    
