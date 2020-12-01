#pragma once

#include <stdlib.h>
#include <math.h>

float random_float() {
    return (rand() + 1.0) / RAND_MAX;
}

float random_float_range(float min, float max) {
    return min + (max - min) * ((rand() + 1.0) / RAND_MAX);
}

float clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

float reflectance(float cosine, float ref_idx) {
    float r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}
