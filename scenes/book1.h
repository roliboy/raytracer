#include "../material.h"
#include "../object.h"
#include "../sphere.h"
#include "../util.h"
#include "../vec3.h"
#include "../scene.h"

scene scene_book1() {
    int r = 11;
    int object_count = 1 + 3 + 4 * r * r;
    int c = 0;

    object* objects = (object*)malloc(sizeof(object) * object_count);
    material* materials = (material*)malloc(sizeof(material) * object_count);

    materials[c] = lambertian_create(vec3_create(0.5, 0.5, 0.5));
    objects[c] = sphere_create(vec3_create(0,-1000,0), 1000, &materials[c]);
    c++;

    for (int a = -r; a < r; a++) {
        for (int b = -r; b < r; b++) {
            float choose_mat = random_float();
            vec3 center = vec3_create(a + 0.9 * random_float(), 0.2, b + 0.9 * random_float());

            if (vec3_norm(vec3_subtract(center, vec3_create(4, 0.2, 0))) > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    vec3 albedo = vec3_multiply_vec3(vec3_random_in_range(0, 1), vec3_random_in_range(0, 1));
                    materials[c] = lambertian_create(albedo);
                    objects[c] = sphere_create(center, 0.2, &materials[c]);
                    c++;
                } else if (choose_mat < 0.95) {
                    // metal
                    vec3 albedo = vec3_random_in_range(0.5, 1);
                    float fuzz = random_float_range(0, 0.5);
                    materials[c] = metal_create(albedo, fuzz);
                    objects[c] = sphere_create(center, 0.2, &materials[c]);
                    c++;

                } else {
                    // glass
                    materials[c] = dielectric_create(1.5);
                    objects[c] = sphere_create(center, 0.2, &materials[c]);
                    c++;
                }
            }
        }
    }


    materials[c] = dielectric_create(1.5);
    objects[c] = sphere_create(vec3_create(0, 1, 0), 1.0, &materials[c]);
    c++;

    materials[c] = lambertian_create(vec3_create(0.4, 0.2, 0.1));
    objects[c] = sphere_create(vec3_create(-4, 1, 0), 1.0, &materials[c]);
    c++;

    materials[c] = metal_create(vec3_create(0.7, 0.6, 0.5), 0.0);
    objects[c] = sphere_create(vec3_create(4, 1, 0), 1.0, &materials[c]);
    c++;

    return scene_create(objects, materials, object_count);
}
