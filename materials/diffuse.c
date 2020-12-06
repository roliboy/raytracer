#include "diffuse.h"

#include "../ray.h"
#include "../hit.h"

bool diffuse_scatter(diffuse* mat, ray* r_in, hit* record, vector* attenuation, ray* r_out) {
    vector scatter_direction = vector_add(record->n, vector_random_on_unit_sphere());

    if (vector_near_zero(scatter_direction)) {
        scatter_direction = record->n;
    }

    *r_out = ray_create(record->p, scatter_direction, r_in->time);
    *attenuation = mat->albedo;
    return true;
}
