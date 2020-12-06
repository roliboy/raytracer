#pragma once

#include "vector.h"
#include "ray.h"

typedef struct bounding_box {
    vector minimum;
    vector maximum;
} bounding_box;


bounding_box bounding_box_create(vector minimum, vector maximum);
bool bounding_box_hit(bounding_box* box, ray* r, float t_min, float t_max);
bool bounding_box_hit2(bounding_box *box, ray *r, float t_min, float t_max);
bounding_box bounding_box_surround(bounding_box box0, bounding_box box1);
