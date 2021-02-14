#include "diffuse_light.h"
#include "../texture.h"

bool diffuse_light_scatter(diffuse_light* mat, ray* r_in, hit* record, vector* attenuation, ray* r_out) {
    return false;
}

vector diffuse_light_emit(diffuse_light* mat, float u, float v, vector p) {
    return texture_value(mat->emit, u, v, p);
}
