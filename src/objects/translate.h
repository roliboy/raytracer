#pragma once

#include "../vector.h"

typedef struct ray ray;
typedef struct hit hit;
typedef struct object object;
typedef struct material material;
typedef struct bounding_box bounding_box;

typedef struct translate {
  object *inner;
  vector offset;
} translate;

object *translate_create(object *inner, vector offset);
bool translate_hit(translate *tr, ray *r, float t_min, float t_max,
                   hit *record);
bool translate_bounding_box(translate *tr, float time0, float time1,
                            bounding_box *box);
