#pragma once

#include "../hit.h"
#include "../ray.h"
#include "../vector.h"
#include "../material.h"
#include "../bounding_box.h"

typedef struct moving_sphere {
    vector center0;
    vector center1;
    float time0;
    float time1;
    float radius;
    material* mat;
} moving_sphere;

bool moving_sphere_hit(moving_sphere* s, ray* r, float t_min, float t_max, hit* record);
bool moving_sphere_bounding_box(moving_sphere *s, float time0, float time1, bounding_box *box);
