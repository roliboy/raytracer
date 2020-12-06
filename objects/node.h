#pragma once

#include "../bounding_box.h"
#include "../ray.h"
#include "../hit.h"
#include <stdbool.h>

typedef struct object object;
typedef struct node {
    object* left;
    object* right;
    bounding_box bounds;
} node;

bool node_hit(node* n, ray* r, float t_min, float t_max, hit* record);
bool node_bounding_box(node* n, float time0, float time1, bounding_box* box);
