#pragma once

#include "vector.h"

typedef struct ray {
  vector origin;
  vector direction;
  float time;
} ray;

ray ray_create(vector origin, vector direction, float time);
vector ray_origin(ray r);
vector ray_direction(ray r);
vector ray_at(ray *r, float t);
