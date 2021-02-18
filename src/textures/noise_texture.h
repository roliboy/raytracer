#pragma once

#include "../noise.h"

typedef struct texture texture;
typedef struct vector vector;

typedef struct noise_texture {
  noise n;
  float scale;
} noise_texture;

texture *noise_texture_create(float scale);
vector noise_texture_value(noise_texture *nt, vector p);
