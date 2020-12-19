#pragma once

#include "../vector.h"

typedef struct ray ray;
typedef struct hit hit;
typedef struct object object;
typedef struct material material;
typedef struct bounding_box bounding_box;

typedef struct xy_rectangle {
    float x0, x1, y0, y1, k;
    material* mat;
} xy_rectangle;

typedef struct yz_rectangle {
    float y0, y1, z0, z1, k;
    material* mat;
} yz_rectangle;

typedef struct zx_rectangle {
    float z0, z1, x0, x1, k;
    material* mat;
} zx_rectangle;

object xy_rectangle_create(float x0, float x1, float y0, float y1, float k, material* mat);
bool xy_rectangle_hit(xy_rectangle* rect, ray* r, float t_min, float t_max, hit* record);
bool xy_rectangle_bounding_box(xy_rectangle* rect, float time0, float time1, bounding_box* box);
object yz_rectangle_create(float y0, float y1, float z0, float z1, float k, material* mat);
bool yz_rectangle_hit(yz_rectangle* rect, ray* r, float t_min, float t_max, hit* record);
bool yz_rectangle_bounding_box(yz_rectangle* rect, float time0, float time1, bounding_box* box);
object zx_rectangle_create(float z0, float z1, float x0, float x1, float k, material* mat);
bool zx_rectangle_hit(zx_rectangle* rect, ray* r, float t_min, float t_max, hit* record);
bool zx_rectangle_bounding_box(zx_rectangle* rect, float time0, float time1, bounding_box* box);
