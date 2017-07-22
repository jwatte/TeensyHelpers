#if !defined(Touch7834_h)
#define Touch7834_h

#include <stdint.h>

class Touch7843 {
  public:
    Touch7843(uint8_t cs = 8, uint32_t clock = 1500000);
    void begin();
    bool read(int16_t *x, int16_t *y);
    void setCalibration(int16_t x0, int16_t x1, int16_t y0, int16_t y1);
private:
    uint32_t clock_;
    uint8_t pin_;
    int32_t xmin, xmax;
    int16_t ymin, ymax;
};

#endif  //  Touch7834_h
