#pragma once

#include "objects/sphere.h"
#include "objects/moving_sphere.h"
#include "material.h"
#include "vector.h"
#include <stdbool.h>

typedef enum object_id {
    object_sphere = 0,
    object_moving_sphere = 1
} object_id;

typedef union object_data {
    sphere sphere;
    moving_sphere moving_sphere;
} object_data;

typedef struct object {
    object_id id;
    object_data data;
} object;

object sphere_create(vector center, float radius, material mat) {
    return (object) {
        .id = object_sphere,
        .data = (object_data) {
            .sphere = (sphere) {
                .center = center,
                .radius = radius,
                .mat = mat
            }
        }
    };
}

object moving_sphere_create(vector center0, vector center1, float time0, float time1, float radius, material mat) {
    return (object) {
        .id = object_moving_sphere,
        .data = (object_data) {
            .moving_sphere = (moving_sphere) {
                .center0 = center0,
                .center1 = center1,
                .time0 = time0,
                .time1 = time1,
                .radius = radius,
                .mat = mat
            }
        }
    };
}

bool object_hit(object* obj, ray* r, float t_min, float t_max, hit* record) {
    switch (obj->id) {
        case object_sphere:
            return sphere_hit(&obj->data.sphere, r, t_min, t_max, record);
        case object_moving_sphere:
            return moving_sphere_hit(&obj->data.moving_sphere, r, t_min, t_max, record);
        default:
            exit(-1);
    }
}

/*
bool sphere_bounding_box(sphere* s, float time0, float time1, bounding_box* box) {
    box->minimum = vector_subtract(s->center, vector_create(s->radius, s->radius, s->radius));
    box->maximum = vector_add(s->center, vector_create(s->radius, s->radius, s->radius));
    return true;
}

bool moving_sphere_bounding_box(moving_sphere *s, float time0, float time1, bounding_box *box) {
    vector center0 = vector_add(
            s->center0,
            vector_multiply_scalar(
                vector_subtract(s->center1, s->center0),
                (time0 - s->time0) / (s->time1 - s->time0)));

    vector center1 = vector_add(
            s->center0,
            vector_multiply_scalar(
                vector_subtract(s->center1, s->center0),
                (time1 - s->time0) / (s->time1 - s->time0)));

    vector radius = vector_create(s->radius, s->radius, s->radius);

    *box = bounding_box_surround(
            bounding_box_create(vector_subtract(center0, radius), vector_add(center0, radius)),
            bounding_box_create(vector_subtract(center1, radius), vector_add(center1, radius)));
}

bool object_bounding_box(object* obj, float time0, float time1, bounding_box* box) {
    switch (obj->id) {
        case object_sphere:
            return sphere_bounding_box(&obj->data.sphere, time0, time1, box);
        case object_moving_sphere:
            return moving_sphere_bounding_box(&obj->data.moving_sphere, time0, time1, box);
//        case object_bounding_box_node:
//            return bounding_box_node_bounding_box(&obj->data.bounding_box, time0, time1, box);
        default:
            break;
    }
}*/

