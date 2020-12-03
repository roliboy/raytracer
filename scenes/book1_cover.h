#include "../material.h"
#include "../object.h"
#include "../sphere.h"
#include "../util.h"
#include "../vector.h"
#include "../scene.h"

void scene_book1() {
    printf("sphere %f %f %f %f diffuse %f %f %f\n", 0.0, -1000.0, 0.0, 1000.0, 0.5, 0.5, 0.5);

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = random_float();
            vector center = vector_create(a + 0.5 * random_float(), 0.2, b + 0.5 * random_float());

            if (vector_norm(vector_subtract(center, vector_create(4, 0.2, 0))) > 0.9) {
                if (choose_mat < 0.8) {
                    vector albedo = vector_multiply(vector_random_in_range(0, 1), vector_random_in_range(0, 1));
                    printf("sphere %f %f %f %f diffuse %f %f %f\n", center.x, center.y, center.z, 0.2, albedo.x, albedo.y, albedo.z);
                } else if (choose_mat < 0.95) {
                    vector albedo = vector_random_in_range(0.5, 1);
                    float fuzz = random_float_range(0, 0.5);
                    printf("sphere %f %f %f %f metal %f %f %f %f\n", center.x, center.y, center.z, 0.2, albedo.x, albedo.y, albedo.z, fuzz);
                } else {
                    printf("sphere %f %f %f %f dielectric %f\n", center.x, center.y, center.z, 0.2, 1.5);
                }
            }
        }
    }

    printf("sphere %f %f %f %f dielectric %f\n", 0.0, 1.0, 0.0, 1.0, 1.5);
    printf("sphere %f %f %f %f diffuse %f %f %f\n", -4.0, 1.0, 0.0, 1.0, 0.4, 0.2, 0.1);
    printf("sphere %f %f %f %f metal %f %f %f %f\n", 4.0, 1.0, 0.0, 1.0, 0.7, 0.6, 0.5, 0.0);
}
