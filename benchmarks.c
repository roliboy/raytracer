#include <stdio.h>
#include <stdbool.h>
#include "material.h"
#include "object.h"
#include "sphere.h"
#include "camera.h"
#include "util.h"
#include "vec3.h"
#include "ray.h"
#include "scenes/book1.h"
#include "hit.h"

#include <inttypes.h>
#include <math.h>
#include <time.h>

long int timestamp() {
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    return round(spec.tv_sec * 1000 + spec.tv_nsec / 1.0e6);
}

vec3 color(ray* r, scene* world, int depth) {
    if (depth <= 0)
        return vec3_create(0, 0, 0);

    hit record;
    if (scene_hit(world, r, 0.001, 0x1.fffffep+127f, &record)) {
        ray scattered;
        vec3 attenuation;
        if (material_scatter(record.mat, r, &record, &attenuation, &scattered))
            return vec3_multiply_vec3(
                        attenuation,
                        color(&scattered, world, depth - 1)
                    );
        return vec3_create(0, 0, 0);
    }

    vec3 unit_direction = vec3_normalize(r->direction);
    float t = 0.5 * (unit_direction.y + 1.0);
    return vec3_add(
            vec3_multiply(
                vec3_create(1.0, 1.0, 1.0), 1.0 - t),
            vec3_multiply(
                vec3_create(0.5, 0.7, 1.0), t)
            );
}

void run(int image_width, int samples_per_pixel, int max_depth, camera* cam, scene* scn) {
    float aspect_ratio = 16.0 / 9.0;
    int image_height = (int)(image_width / aspect_ratio);

    #pragma omp parallel for
    for (int y = image_height - 1; y >= 0; y--) {
        for (int x = 0; x < image_width; x++) {
            vec3 pixel_color = vec3_create(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                float u = ((float)x + random_float() - 0.5) / (image_width - 1);
                float v = ((float)y + random_float() - 0.5) / (image_height - 1);

                ray r = camera_get_ray(cam, u, v);
                pixel_color = vec3_add(pixel_color, color(&r, scn, max_depth));
            }
        }
    }
}

void test(int image_width, int samples_per_pixel, int max_depth, camera* cam, scene* scn) {
    long start = timestamp();
    run(image_width, samples_per_pixel, max_depth, cam, scn);
    long end = timestamp();

    printf("running %dx%d with %d samples per pixel and %d max reflections took %ldms\n",
            image_width,
            (int)(image_width * 9.0 / 16.0),
            samples_per_pixel,
            max_depth,
            end - start);
}

int main() {
    scene scn = scene_book1();

    vec3 lookfrom = vec3_create(13, 2, 3);
    vec3 lookat = vec3_create(0, 0, 0);
    vec3 vup = vec3_create(0, 1, 0);
    float dist_to_focus = 10;
    float aperture = 0.1;

    camera cam = camera_create(lookfrom, lookat, vup, 20, 16.0 / 9.0, aperture, dist_to_focus);

    test(1920 / 8, 4, 4, &cam, &scn);

    scene_destroy(&scn);

/*    for (int scale = 8; scale >= 2; scale /= 2)
    for (int samples = 1; samples <= 4; samples *= 2)
    for (int max_depth = 1; max_depth <= 64; max_depth *= 2)
        test(1920 / scale, samples, max_depth, &cam, &scn);*/
}
