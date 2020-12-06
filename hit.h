#pragma once

#include "vector.h"

typedef struct material material;
typedef struct ray ray;
typedef struct hit {
    float t;
    vector p;
    vector n;
    bool front_face;
    material* mat;
} hit;

void hit_set_face_normal(hit* h, ray* r, vector outward_normal);
