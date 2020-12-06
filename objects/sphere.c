#include "sphere.h"

#include "../ray.h"
#include "../vector.h"
#include "../material.h"
#include "../bounding_box.h"
#include <math.h>

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

    vector outward_normal = vector_divide_scalar(vector_subtract(record->p, s->center), s->radius);
    hit_set_face_normal(record, r, outward_normal);
    record->mat = &s->mat;

    return true;
}

bool sphere_bounding_box(sphere* s, float time0, float time1, bounding_box* box) {
    //TODO: use constructor
    box->minimum = vector_subtract(s->center, vector_create(s->radius, s->radius, s->radius));
    box->maximum = vector_add(s->center, vector_create(s->radius, s->radius, s->radius));
    return true;
}
