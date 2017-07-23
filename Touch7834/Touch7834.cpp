#include <Arduino.h>
#include "Touch7834.h"
#include "SPI.h"

#define CMD_RDX  0xD0
#define CMD_RDY  0x90
#define ITERS       9
#define JITTER      9

static void sort(int16_t *a) {
  for (int i = 0; i != ITERS-1; ++i) {
    int smix = i;
    int smval = a[i];
    for (int j = i+1; j != ITERS; ++j) {
      if (a[j] < smval) {
        smix = j;
        smval = a[j];
      }
    }
    if (smix != i) {
      int16_t v = a[i];
      a[i] = smval;
      a[smix] = v;
    }
  }
}

Touch7834::Touch7834(uint8_t cs, uint32_t clock) : clock_(clock), pin_(cs) {
    pinMode(pin_, OUTPUT);
    digitalWrite(pin_, HIGH);
    xmin = 1600;
    xmax = 30000;
    ymin = 1600;
    ymax = 30000;
}

void Touch7834::begin() {
}

bool Touch7834::read(int16_t *x, int16_t *y) {
    uint16_t ux, uy;
    int16_t cx[ITERS];
    int16_t cy[ITERS];
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
        if (uy < ymin || uy > ymax) {
            got = false;
        }
        if (ux < xmin || ux > xmax) {
            got = false;
        }
        cy[i] = 239 - 240 * (uy - ymin) / (ymax - ymin);
        cx[i] = 319 - 320 * (ux - xmin) / (xmax - xmin);
    }
    sort(cx);
    sort(cy);
    if (cx[ITERS-1] - cx[0] > JITTER) {
      got = false;
    }
    if (cy[ITERS-1] - cy[0] > JITTER) {
      got = false;
    }
    *x = cx[ITERS/2];
    *y = cy[ITERS/2];
    return got;
}

void Touch7834::setCalibration(int16_t x0, int16_t x1, int16_t y0, int16_t y1) {
    xmin = x0;
    xmax = x1;
    ymin = y0;
    ymax = y1;
}
