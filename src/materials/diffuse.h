#pragma once

#include <stdbool.h>

#include "../texture.h"

typedef struct hit hit;
typedef struct ray ray;
typedef struct material material;

typedef struct diffuse {
  // vector albedo;
  texture *albedo;
} diffuse;

material diffuse_create(texture *albedo);
bool diffuse_scatter(diffuse *mat, ray *r_in, hit *record, vector *attenuation,
                     ray *r_out);
