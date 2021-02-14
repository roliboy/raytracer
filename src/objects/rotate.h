#pragma once

#include "../vector.h"
#include "../bounding_box.h"

typedef struct ray ray;
typedef struct hit hit;
typedef struct object object;
typedef struct material material;

typedef struct rotate_y {
    object* inner;
    float sin_theta;
    float cos_theta;
    bool hasbox;
    bounding_box box;
} rotate_y;

object rotate_y_create(object* inner, float angle);
bool rotate_y_hit(rotate_y* ry, ray* r, float t_min, float t_max, hit* record);
bool rotate_y_bounding_box(rotate_y* ry, float time0, float time1, bounding_box* box);
