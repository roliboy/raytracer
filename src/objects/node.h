#pragma once

#include <stdbool.h>

#include "../bounding_box.h"

typedef struct object object;
typedef struct hit hit;

typedef struct node {
    object* left;
    object* right;
    bounding_box bounds;
} node;

object* node_create(object** objects, int start, int end, float time0, float time1);
bool node_hit(node* n, ray* r, float t_min, float t_max, hit* record);
bool node_bounding_box(node* n, float time0, float time1, bounding_box* box);
