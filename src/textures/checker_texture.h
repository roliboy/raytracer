#pragma once

#include "../vector.h"

typedef struct texture texture;

typedef struct checker_texture {
  vector even;
  vector odd;
} checker_texture;

texture checker_texture_create(vector even, vector odd);
vector checker_texture_value(checker_texture *ct, float u, float v, vector p);
