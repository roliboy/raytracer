#include <stdio.h>
#include "framebuffer.h"
#include "camera.h"
#include "object.h"
#include "util.h"
#include "vector.h"
#include "ray.h"
#include "scene.h"
#include "hit.h"

vector color(ray* r, scene* world, int depth) {
    if (depth <= 0)
        return vector_create(0, 0, 0);

    hit record;
    if (scene_hit(world, r, 0.001, 0x1.fffffep+127f, &record)) {
        ray scattered;
        vector attenuation;
        if (material_scatter(record.mat, r, &record, &attenuation, &scattered))
            return vector_multiply(
                    attenuation,
                    color(&scattered, world, depth - 1)
                    );
        return vector_create(0, 0, 0);
    }

    vector unit_direction = vector_normalize(r->direction);
    float t = 0.5 * (unit_direction.y + 1.0);
    return vector_add(
            vector_multiply_scalar(
                vector_create(1.0, 1.0, 1.0), 1.0 - t),
            vector_multiply_scalar(
                vector_create(0.5, 0.7, 1.0), t)
            );
}

int main() {
    float aspect_ratio = 16.0 / 9.0;
    int image_width = 1920 / 8;
    int image_height = (int)(image_width / aspect_ratio);
    int samples_per_pixel = 32;
    int max_depth = 8;


    scene scn = scene_load("scenes/book1_cover_overkill.scn");

    vector lookfrom = vector_create(13, 2, 3);
    vector lookat = vector_create(0, 0, 0);
    vector vup = vector_create(0, 1, 0);
    float dist_to_focus = 10;
    float aperture = 0.1;

    camera cam = camera_create(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    framebuffer fb = framebuffer_create(image_width, image_height);

    #pragma omp parallel for
    for (int y = image_height - 1; y >= 0; y--) {
        for (int x = 0; x < image_width; x++) {
            vector pixel_color = vector_create(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                float u = ((float)x + random_float(0, 1) - 0.5) / (image_width - 1);
                float v = ((float)y + random_float(0, 1) - 0.5) / (image_height - 1);

                ray r = camera_get_ray(&cam, u, v);
                pixel_color = vector_add(pixel_color, color(&r, &scn, max_depth));

            }
            vector rgb = vector_rgb(pixel_color, samples_per_pixel);
            framebuffer_set(&fb, x, image_height - 1 - y, rgb);
        }
    }

    scene_destroy(&scn);
}
