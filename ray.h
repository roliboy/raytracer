#pragma once

#include "vector.h"

typedef struct ray {
    vector origin;
    vector direction;
} ray;

ray ray_create(vector origin, vector direction) {
    return (ray) {origin, direction};
}

vector ray_origin(ray r) {
    return r.origin;
}

vector ray_direction(ray r) {
    return r.direction;
}

vector ray_at(ray* r, float t) {
    return vector_add(r->origin, vector_multiply_scalar(r->direction, t));
}
