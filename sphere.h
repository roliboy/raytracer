#pragma once

#include "material.h"
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include <stdio.h>

typedef struct sphere {
    vec3 center;
    float radius;
    material* mat;
} sphere;

bool sphere_hit(sphere* s, ray* r, float t_min, float t_max, hit* record) {
    vec3 oc = vec3_subtract(r->origin, s->center);
    float a = vec3_norm2(r->direction);
    float half_b = vec3_dot(oc, r->direction);
    float c = vec3_norm2(oc) - s->radius * s->radius;

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
    record->n = vec3_divide(vec3_subtract(record->p, s->center), s->radius);

    vec3 outward_normal = vec3_divide(vec3_subtract(record->p, s->center), s->radius);
    hit_set_face_normal(record, r, outward_normal);
    record->mat = s->mat;

//    record->n = vec3_create(0, 1, 0);//(rec.p - center) / radius;

    return true;
}

