#pragma once

#include "util.h"
#include "vector.h"

typedef struct noise {
    vector rnf[256];
    int px[256];
    int py[256];
    int pz[256];
} noise;

noise noise_create();
float noise_get_value(noise* n, vector p);
float noise_get_turbulent(noise* n, vector p, int depth);
