#pragma once

#include "../bounding_box.h"

typedef struct ray ray;
typedef struct hit hit;
typedef struct object object;
typedef struct bounding_box bounding_box;
typedef struct material material;
typedef struct texture texture;

typedef struct constant_medium {
  object *boundary;
  float neg_inv_density;
  material *phase_function;
} constant_medium;

object *constant_medium_create(object *boundary, float density, material *phase);
bool constant_medium_hit(constant_medium *cm, ray *r, float t_min, float t_max,
                         hit *record);
bool constant_medium_bounding_box(constant_medium *cm, float time0, float time1,
                                  bounding_box *box);
