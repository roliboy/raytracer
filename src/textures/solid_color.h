#pragma once

#include "../vector.h"

typedef struct solid_color {
    vector color;
} solid_color;

vector solid_color_value(solid_color *tsc);
