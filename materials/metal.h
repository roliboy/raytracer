#pragma once

#include "../vector.h"
#include <stdbool.h>

typedef struct hit hit;
typedef struct ray ray;
typedef struct metal {
    vector albedo;
    float fuzz;
} metal;

bool metal_scatter(metal* mat, ray* r_in, hit* record, vector* attenuation, ray* r_out);
