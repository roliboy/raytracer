#pragma once

#include "vec3.h"
#include "ray.h"

typedef struct material material;
typedef struct hit {
    float t;
    vec3 p;
    vec3 n;
    bool front_face;
    material* mat;
} hit;

void hit_set_face_normal(hit* h, ray* r, vec3 outward_normal) {
    h->front_face = vec3_dot(r->direction, outward_normal) < 0;
    h->n = h->front_face ? outward_normal : vec3_invert(outward_normal);
}
