#pragma once

#include "../vector.h"

typedef struct checker_texture {
    vector even;
    vector odd;
} checker_texture;

vector checker_texture_value(checker_texture* ct, float u, float v, vector p);
