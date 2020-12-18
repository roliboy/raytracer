#pragma once

#include "../vector.h"

typedef struct texture texture;
typedef struct invert_texture {
    texture* inner;
} invert_texture;

vector invert_texture_value(invert_texture* tex, float u, float v, vector p);
