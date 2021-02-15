#pragma once

#include <stdbool.h>

#include "../texture.h"
#include "../vector.h"

typedef struct hit hit;
typedef struct ray ray;
typedef struct material material;
typedef struct isotropic {
  texture *albedo;
} isotropic;

material isotropic_create(texture *albedo);
bool isotropic_scatter(isotropic *mat, ray *r_in, hit *record,
                       vector *attenuation, ray *r_out);
