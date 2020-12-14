#include "texture.h"
#include "noise.h"
#include "util.h"

texture solid_color_create(vector color) {
    return (texture) {
        .id = texture_solid_color,
        .data = (texture_data) {
            .solid_color = (solid_color) {
                .color = color
            }
        }
    };
}

texture checker_texture_create(vector even, vector odd) {
    return (texture) {
        .id = texture_checker_texture,
        .data = (texture_data) {
            .checker_texture = (checker_texture) {
                .even = even,
                .odd = odd
            }
        }
    };
}

texture noise_texture_create(float scale) {
    noise_texture nt;
    nt.scale = scale;
    nt.n = noise_create();

    return (texture) {
        .id = texture_noise_texture,
        .data = (texture_data) {
            .noise_texture = nt
        }
    };
}

texture image_texture_create(char* file) {
    unsigned int width;
    unsigned int height;
    unsigned char* bytes = load_ppm(file, &width, &height);
    return (texture) {
        .id = texture_image_texture,
        .data = (texture_data) {
            .image_texture = (image_texture) {
                .bytes = bytes,
                .width = width,
                .height = height
            }
        }
    };
}

vector texture_value(texture *t, float u, float v, vector p) {
    switch (t->id) {
        case texture_solid_color:
            return solid_color_value(&t->data.solid_color);
        case texture_checker_texture:
            return checker_texture_value(&t->data.checker_texture, u, v, p);
        case texture_noise_texture:
            return noise_texture_value(&t->data.noise_texture, p);
        case texture_image_texture:
            return image_texture_value(&t->data.image_texture, u, v, p);
        default:
            return vector_create(255, 0, 255);
    }
}
