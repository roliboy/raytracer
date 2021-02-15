#pragma once

#include "../vector.h"

// TODO: actually call image_texture_free
typedef struct texture texture;

typedef struct image_texture {
  unsigned char *bytes;
  unsigned int width;
  unsigned int height;
} image_texture;

texture image_texture_create(char *file);
vector image_texture_value(image_texture *it, float u, float v, vector p);
void image_texture_free(image_texture *it);
