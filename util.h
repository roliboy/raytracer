#pragma once

#include <math.h>
#include <stdlib.h>

float random_float(float min, float max) {
    return min + (max - min) * ((rand() + 1.0) / RAND_MAX);
}

int random_int(int min, int max) {
    return min + rand()%(max - min + 1);
}

float clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

float reflectance(float cosine, float ref_idx) {
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}
