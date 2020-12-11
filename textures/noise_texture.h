#pragma once

#include "../vector.h"
#include "../noise.h"

typedef struct noise_texture {
    noise n;
    float scale;
} noise_texture;

vector noise_texture_value(noise_texture* nt, vector p);
