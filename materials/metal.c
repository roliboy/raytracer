#include "metal.h"

#include "../hit.h"
#include "../ray.h"

bool metal_scatter(metal* mat, ray* r_in, hit* record, vector* attenuation, ray* r_out) {
    vector reflected = vector_reflect(vector_normalize(r_in->direction), record->n);
    *r_out = ray_create(record->p, vector_add(reflected,
                vector_multiply_scalar(vector_random_in_unit_sphere(), mat->fuzz)), r_in->time);
    *attenuation = mat->albedo;
    return vector_dot(r_out->direction, record->n) > 0;
}
