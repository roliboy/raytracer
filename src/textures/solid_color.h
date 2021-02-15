#pragma once

#include "../vector.h"

typedef struct texture texture;

typedef struct solid_color {
  vector color;
} solid_color;

texture solid_color_create(vector color);
vector solid_color_value(solid_color *tsc);
