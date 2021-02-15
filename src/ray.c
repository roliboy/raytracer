#include "ray.h"

ray ray_create(vector origin, vector direction, float time) {
  return (ray){.origin = origin, .direction = direction, .time = time};
}

vector ray_origin(ray r) { return r.origin; }

vector ray_direction(ray r) { return r.direction; }

vector ray_at(ray *r, float t) {
  return vector_add(r->origin, vector_multiply_scalar(r->direction, t));
}
