#pragma once

#include "vector.h"
#include "ray.h"

typedef struct bounding_box {
    vector minimum;
    vector maximum;
} bounding_box;


bounding_box bounding_box_create(vector minimum, vector maximum) {
    return (bounding_box) {
        .minimum = minimum,
        .maximum = maximum
    };
}


bool bounding_box_hit(bounding_box* box, ray* r, float t_min, float t_max) {
    float t0, t1;
    t0 = fmin((box->minimum.x - r->origin.x) / r->direction.x, (box->maximum.x - r->origin.x) / r->direction.x);
    t1 = fmax((box->minimum.x - r->origin.x) / r->direction.x, (box->maximum.x - r->origin.x) / r->direction.x);
    t_min = fmax(t0, t_min);
    t_max = fmin(t1, t_max);
    if (t_max <= t_min)
        return false;

    t0 = fmin((box->minimum.y - r->origin.y) / r->direction.y, (box->maximum.y - r->origin.y) / r->direction.y);
    t1 = fmax((box->minimum.y - r->origin.y) / r->direction.y, (box->maximum.y - r->origin.y) / r->direction.y);
    t_min = fmax(t0, t_min);
    t_max = fmin(t1, t_max);
    if (t_max <= t_min)
        return false;

    t0 = fmin((box->minimum.z - r->origin.z) / r->direction.z, (box->maximum.z - r->origin.z) / r->direction.z);
    t1 = fmax((box->minimum.z - r->origin.z) / r->direction.z, (box->maximum.z - r->origin.z) / r->direction.z);
    t_min = fmax(t0, t_min);
    t_max = fmin(t1, t_max);
    if (t_max <= t_min)
        return false;

    return true;
}

bool bounding_box_hit2(bounding_box *box, ray *r, float t_min, float t_max) {
    float inv, t0, t1;
    inv = 1.0 / r->direction.x;
    t0 = (box->minimum.x - r->origin.x) * inv;
    t1 = (box->maximum.x - r->origin.x) * inv;
    //TODO: branchless swapp
    if (inv < 0) {
        float a = t0;
        t0 = t1;
        t1 = a;
    }
    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;
    if (t_max <= t_min)
        return false;


    inv = 1.0 / r->direction.y;
    t0 = (box->minimum.y - r->origin.y) * inv;
    t1 = (box->maximum.y - r->origin.y) * inv;
    //TODO: branchless swapp
    if (inv < 0) {
        float a = t0;
        t0 = t1;
        t1 = a;
    }
    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;
    if (t_max <= t_min)
        return false;


    inv = 1.0 / r->direction.z;
    t0 = (box->minimum.z - r->origin.z) * inv;
    t1 = (box->maximum.z - r->origin.z) * inv;
    //TODO: branchless swapp
    if (inv < 0) {
        float a = t0;
        t0 = t1;
        t1 = a;
    }
    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;
    if (t_max <= t_min)
        return false;

    return true;
}

bounding_box bounding_box_surround(bounding_box box0, bounding_box box1) {
    vector small = vector_create(
            fmin(box0.minimum.x, box1.minimum.x),
            fmin(box0.minimum.y, box1.minimum.y),
            fmin(box0.minimum.z, box1.minimum.z));

    vector large = vector_create(
            fmax(box0.maximum.x, box1.maximum.x),
            fmax(box0.maximum.y, box1.maximum.y),
            fmax(box0.maximum.z, box1.maximum.z));

    return (bounding_box) {
        .minimum = small,
        .maximum = large
    };
}
