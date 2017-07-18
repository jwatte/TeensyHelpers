# PinPulseIn.h

Usage:

Copy PinPulseIn.h into your project directory.

Include it in your sketch file.

Define a PinPulseIn object per digital pin you want to read, with the digital pin number as an argument.

In your setup() function, call pin_.init();

In your loop() function, call getValue(); if it returns a value > 0, then there was a signal in the range 500 to 2500 microseconds (extended servo range) and that value was returned.

Note that getValue() will only return the value once per pulse that arrives, so you will want to store it in some other variable. This is so that you don't end up re-using stale values after the RC receiver has turned off.

    #include <PinPulseIn.h>
    
    PinPulseIn<9> pin9_; // RC PWM input on pin 9
    
    void setup() {
        pin9_.init();
    }
    
    void loop() {
        uint16_t value = pin9_.getValue();
        if (value) {
            SerialUSB.println(value);
        }
        delay(20); // or whatever else your sketch is doing
    }

