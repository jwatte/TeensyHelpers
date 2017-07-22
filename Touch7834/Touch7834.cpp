#include <Arduino.h>
#include "Touch7834.h"

#define  CMD_RDX  0xD0
#define  CMD_RDY  0x90
#define ITERS 4

Touch7843::Touch7843(uint8_t cs = 8, uint32_t clock = 1500000) : clock_(clock), pin_(cs) {
    pinMode(pin_, OUTPUT);
    digitalWrite(pin_, HIGH);
    xmin = 2000;
    xmax = 30600;
    ymin = 1400;
    ymax = 29400;
}

void Touch7843::begin() {
}

bool Touch7843::read(int16_t *x, int16_t *y) {
    float xx = 0;
    float yy = 0;
    int16_t ux, uy;
    bool got = true;
    for (int i = 0; i != ITERS; ++i) {
        digitalWrite(pin_, LOW);
        SPI.beginTransaction(SPISettings(clock_, MSBFIRST, SPI_MODE0));
        SPI.transfer(CMD_RDX);
        uy = SPI.transfer(0) << 8;
        uy |= SPI.transfer(0);
        SPI.endTransaction();
        digitalWrite(pin_, HIGH);
        delayMicroseconds(10);
        digitalWrite(pin_, LOW);
        SPI.beginTransaction(SPISettings(clock_, MSBFIRST, SPI_MODE0));
        SPI.transfer(CMD_RDY);
        ux = SPI.transfer(0) << 8;
        ux |= SPI.transfer(0);
        SPI.endTransaction();
        digitalWrite(pin_, HIGH);
        if (uy < 1400 || uy > 29400) {
            got = false;
        }
        if (ux < 2000 || ux > 30600) {
            got = false;
        }
        yy += 239 - 240 * (uy - 1400) / (29400 - 1400);
        xx += 319 - 320 * (ux - 2000) / (30600 - 2000);
    }
    *x = (uint16_t)(xx / ITERS);
    *y = (uint16_t)(yy / ITERS);
    return got;
}

void Touch7834::setCalibration(int16_t x0, int16_t x1, int16_t y0, int16_t y1) {
    xmin = x0;
    xmax = x1;
    ymin = y0;
    ymax = y1;
}
