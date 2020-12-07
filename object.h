#pragma once

#include "objects/node.h"
#include "objects/sphere.h"
#include "objects/moving_sphere.h"
#include "material.h"
#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>

typedef enum object_id {
    object_node = 0,
    object_sphere = 1,
    object_moving_sphere = 2
} object_id;

typedef union object_data {
    node node;
    sphere sphere;
    moving_sphere moving_sphere;
} object_data;

typedef struct object {
    object_id id;
    object_data data;
} object;

object sphere_create(vector center, float radius, material mat);
object moving_sphere_create(vector center0, vector center1, float time0, float time1, float radius, material mat);
bool object_bounding_box(object* obj, float time0, float time1, bounding_box* box);
object* node_create(object* objects, object* tree, int start, int end, float time0, float time1);
bool object_hit(object* obj, ray* r, float t_min, float t_max, hit* record);
int bounding_box_x_compare(const void*, const void*);
int bounding_box_y_compare(const void*, const void*);
int bounding_box_z_compare(const void*, const void*);

