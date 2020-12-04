#pragma once

#include "../ray.h"
#include "../vector.h"
#include "../material.h"

typedef struct sphere {
    vector center;
    float radius;
    material mat;
} sphere;

bool sphere_hit(sphere* s, ray* r, float t_min, float t_max, hit* record) {
    vector oc = vector_subtract(r->origin, s->center);
    float a = vector_norm_squared(r->direction);
    float half_b = vector_dot(oc, r->direction);
    float c = vector_norm_squared(oc) - s->radius * s->radius;

    float discriminant = half_b * half_b - a * c;

    if (discriminant < 0) return false;

    float sqrtd = sqrt(discriminant);

    float root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    record->t = root;
    record->p = ray_at(r, record->t);


    //TODO: remove?
    record->n = vector_divide_scalar(vector_subtract(record->p, s->center), s->radius);

    vector outward_normal = vector_divide_scalar(vector_subtract(record->p, s->center), s->radius);
    hit_set_face_normal(record, r, outward_normal);
    record->mat = &s->mat;

    return true;
}
