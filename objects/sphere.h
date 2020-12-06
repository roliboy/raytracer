#pragma once

#include "../hit.h"
#include "../ray.h"
#include "../vector.h"
#include "../material.h"
#include "../bounding_box.h"

typedef struct sphere {
    vector center;
    float radius;
    material mat;
} sphere;

bool sphere_hit(sphere* s, ray* r, float t_min, float t_max, hit* record);
bool sphere_bounding_box(sphere* s, float time0, float time1, bounding_box* box);
