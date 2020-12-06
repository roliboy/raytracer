#pragma once

#include "../vector.h"
#include <stdbool.h>

typedef struct hit hit;
typedef struct ray ray;
typedef struct diffuse {
    vector albedo;
} diffuse;

bool diffuse_scatter(diffuse* mat, ray* r_in, hit* record, vector* attenuation, ray* r_out);
