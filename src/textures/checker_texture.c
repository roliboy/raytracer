#include "checker_texture.h"
#include "../texture.h"
#include <math.h>

vector checker_texture_value(checker_texture* ct, float u, float v, vector p) {
    float sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
    if (sines < 0)
        return ct->odd;
    else
        return ct->even;
}
