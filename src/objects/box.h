#pragma once

#include "../vector.h"
#include "rectangle.h"

typedef struct ray ray;
typedef struct hit hit;
typedef struct object object;
typedef struct material material;
typedef struct bounding_box bounding_box;

typedef struct box {
    vector min;
    vector max;
    xy_rectangle back;
    xy_rectangle front;
    yz_rectangle left;
    yz_rectangle right;
    zx_rectangle bottom;
    zx_rectangle top;
    material* mat;
} box;

object box_create(vector min, vector max, material* mat);
bool box_hit(box* rect, ray* r, float t_min, float t_max, hit* record);
bool box_bounding_box(box* rect, float time0, float time1, bounding_box* box);
