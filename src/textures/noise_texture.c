#include "noise_texture.h"
#include <math.h>

vector noise_texture_value(noise_texture* nt, vector p) {
    return vector_multiply_scalar(
            vector_create(1, 1, 1),
            0.5 * (1 + sin(nt->scale * p.z + 10 * noise_get_turbulent(&nt->n, p, 7))));
    return vector_multiply_scalar(
            vector_create(1, 1, 1),
            noise_get_turbulent(
                &nt->n,
                vector_multiply_scalar(p, nt->scale),
                7));
    return vector_multiply_scalar(
            vector_create(1, 1, 1),
            0.5 * (1.0 + noise_get_value(
                &nt->n,
                vector_multiply_scalar(p, nt->scale))));
}
