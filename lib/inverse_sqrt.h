#ifndef INVERSE_SQRT_H
#define INVERSE_SQRT_H

#include "absolute_cast.h"

float InverseSqrt (float x) {
    float xhalf = 0.5f * x;
    int i = absolute_cast<int>(x);
    i = 0x5f3759df - (i>>1);
    x = absolute_cast<float>(i);
    return x * (1.5f - xhalf * x * x);
}


#endif /* INVERSE_SQRT_H */
