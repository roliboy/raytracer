#pragma once

#include "../vector.h"
#include "../texture.h"
#include <stdbool.h>

typedef struct hit hit;
typedef struct ray ray;
typedef struct diffuse_light {
    texture* emit;
} diffuse_light;

bool diffuse_light_scatter(diffuse_light* mat, ray* r_in, hit* record, vector* attenuation, ray* r_out);
vector diffuse_light_emit(diffuse_light* mat, float u, float v, vector p);
