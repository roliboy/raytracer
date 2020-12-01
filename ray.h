#pragma once

#include "vec3.h"

typedef struct ray {
    vec3 origin;
    vec3 direction;
} ray;

ray ray_create(vec3 origin, vec3 direction) {
    return (ray) {origin, direction};
}

vec3 ray_origin(ray r) {
    return r.origin;
}

vec3 ray_direction(ray r) {
    return r.direction;
}

vec3 ray_at(ray* r, float t) {
    return vec3_add(r->origin, vec3_multiply(r->direction, t));
}
