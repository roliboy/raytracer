#include <stdio.h>
#include <stdbool.h>
#include "material.h"
#include "object.h"
#include "sphere.h"
#include "camera.h"
#include "util.h"
#include "vec3.h"
#include "ray.h"
#include "scene.h"
#include "hit.h"

//TODO: leak
int random_scene(object* objects) {
    material* ground_material = (material*)malloc(sizeof(material));
    *ground_material = lambertian_create(vec3_create(0.5, 0.5, 0.5));
    objects[0] = sphere_create(vec3_create(0,-1000,0), 1000, ground_material);
    int c = 1;

    int r = 11;
    for (int a = -r; a < r; a++) {
        for (int b = -r; b < r; b++) {
            float choose_mat = random_float();
            vec3 center = vec3_create(a + 0.9 * random_float(), 0.2, b + 0.9 * random_float());

            if (vec3_norm(vec3_subtract(center, vec3_create(4, 0.2, 0))) > 0.9) {
                material* sphere_material = (material*)malloc(sizeof(material));

                if (choose_mat < 0.8) {
                    // diffuse
                    vec3 albedo = vec3_multiply_vec3(vec3_random_in_range(0, 1), vec3_random_in_range(0, 1));
                    *sphere_material = lambertian_create(albedo);
                    objects[c++] = sphere_create(center, 0.2, sphere_material);
                } else if (choose_mat < 0.95) {
                    // metal
                    vec3 albedo = vec3_random_in_range(0.5, 1);
                    float fuzz = random_float(0, 0.5);
                    *sphere_material = metal_create(albedo, fuzz);
                    objects[c++] = sphere_create(center, 0.2, sphere_material);
                } else {
                    // glass
                    *sphere_material = dielectric_create(1.5);
                    objects[c++] = sphere_create(center, 0.2, sphere_material);
                }
            }
        }
    }


    material* material1 = (material*)malloc(sizeof(material));
    *material1 = dielectric_create(1.5);
    objects[c++] = sphere_create(vec3_create(0, 1, 0), 1.0, material1);

    material* material2 = (material*)malloc(sizeof(material));
    *material2 = lambertian_create(vec3_create(0.4, 0.2, 0.1));
    objects[c++] = sphere_create(vec3_create(-4, 1, 0), 1.0, material2);

    material* material3 = (material*)malloc(sizeof(material));
    *material3 = metal_create(vec3_create(0.7, 0.6, 0.5), 0.0);
    objects[c++] = sphere_create(vec3_create(4, 1, 0), 1.0, material3);
    return c;
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

int main() {
    float aspect_ratio = 16.0 / 9.0;
    int image_width = 1920 / 8;
    int image_height = (int)(image_width / aspect_ratio);
    int samples_per_pixel = 16;
    int max_depth = 8;

    printf("P3\n%d %d\n255\n", image_width, image_height);

    object randomobjects[512];
    int c = random_scene(randomobjects);

    scene scn = scene_create(randomobjects, c);

    vec3 lookfrom = vec3_create(13, 2, 3);
    vec3 lookat = vec3_create(0, 0, 0);
    vec3 vup = vec3_create(0, 1, 0);
    float dist_to_focus = 10;
    float aperture = 0.1;

    camera cam = camera_create(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    for (int y = image_height - 1; y >= 0; y--) {
        fprintf(stderr, "%d scanlines left\n", y);
//        #pragma omp parallel for
        for (int x = 0; x < image_width; x++) {
            //fprintf(stderr, "%d pixels left\n", image_width - x);

            vec3 pixel_color = vec3_create(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                float u = ((float)x + random_float() - 0.5) / (image_width - 1);
                float v = ((float)y + random_float() - 0.5) / (image_height - 1);

                ray r = camera_get_ray(&cam, u, v);
                pixel_color = vec3_add(pixel_color, color(&r, &scn, max_depth));
            }

            vec3_print_color(pixel_color, samples_per_pixel);
            printf(" ");
        }
        printf("\n");
    }
}
