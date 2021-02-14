#include "invert_texture.h"
#include "../texture.h"

vector invert_texture_value(invert_texture* tex, float u, float v, vector p) {
    vector inner_value = texture_value(tex->inner, u, v, p);
    vector inverted = vector_create(1 - inner_value.x, 1 - inner_value.y, 1 - inner_value.z);
    return vector_multiply(vector_multiply(inverted, inverted), vector_multiply(inverted, inverted));
}
