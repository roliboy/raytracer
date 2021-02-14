#include "image_texture.h"
#include "../util.h"
#include <stdlib.h>

vector image_texture_value(image_texture *it, float u, float v, vector p) {
    u = clamp(u, 0.0, 1.0);
    v = 1.0 - clamp(v, 0.0, 1.0);

    int i = (int)(u * it->width);
    int j = (int)(v * it->height);

    if (i >= it->width) i = it->width - 1;
    if (j >= it->height) j = it->height - 1;

    float color_scale = 1.0 / 255.0;
    int pixel_index = j * 3 * it->width + i * 3;

    return vector_create(
            color_scale * it->bytes[pixel_index + 0],
            color_scale * it->bytes[pixel_index + 1],
            color_scale * it->bytes[pixel_index + 2]);
}

void image_texture_free(image_texture *it) {
    free(it->bytes);
}
