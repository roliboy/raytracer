#pragma once

#include <stdbool.h>

#include "../texture.h"
#include "../vector.h"

typedef struct hit hit;
typedef struct ray ray;
typedef struct material material;

typedef struct diffuse_light {
  texture *emit;
} diffuse_light;

material diffuse_light_create(texture *emit);
bool diffuse_light_scatter(diffuse_light *mat, ray *r_in, hit *record,
                           vector *attenuation, ray *r_out);
vector diffuse_light_emit(diffuse_light *mat, float u, float v, vector p);
