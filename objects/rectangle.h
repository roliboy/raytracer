#pragma once

#include "../vector.h"

typedef struct ray ray;
typedef struct hit hit;
typedef struct object object;
typedef struct material material;
typedef struct bounding_box bounding_box;
typedef struct rectangle {
    float x0, x1, y0, y1, k;
    material* mat;
} rectangle;

bool rectangle_hit(rectangle* rect, ray* r, float t_min, float t_max, hit* record);
bool rectangle_bounding_box(rectangle* rect, float time0, float time1, bounding_box* box);
object rectangle_create(float x0, float y0, float x1, float y1, float k, material* mat);
