#pragma once

#include <stdbool.h>

#include "../vector.h"

typedef struct hit hit;
typedef struct ray ray;
typedef struct material material;

typedef struct metal {
  vector albedo;
  float fuzz;
} metal;

material *metal_create(vector albedo, float fuzz);
bool metal_scatter(metal *mat, ray *r_in, hit *record, vector *attenuation,
                   ray *r_out);
