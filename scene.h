#pragma once

#include <string.h>
#include "hit.h"
#include "material.h"
#include "object.h"
#include "sphere.h"
#include "vector.h"

typedef struct scene {
    object* objects;
    int size;
} scene;

scene scene_load(char* filename) {
    FILE* fp;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    char tag[128];
    int object_count = 0;
    while (fscanf(fp, "%s", tag) == 1) {
        if (!strcmp(tag, "sphere")) object_count++;
    }

    object* objects = (object*)malloc(sizeof(object) * object_count);
    int c = 0;

    rewind(fp);

    char type[32];
    while (fscanf(fp, "%s", type) == 1) {
        if (!strcmp(type, "sphere")) {
            float x, y, z, rad;
            fscanf(fp, "%f %f %f %f", &x, &y, &z, &rad);
            char mat[32];
            fscanf(fp, "%s", mat);
            if (!strcmp(mat, "lambertian")) {
                float r, g, b;
                fscanf(fp, "%f %f %f", &r, &g, &b);
                //printf("%s %f %f %f %s %f %f %f\n", type, x, y, z, mat, r, g, b);
                material mat = lambertian_create(vector_create(r, g, b));
                objects[c] = sphere_create(vector_create(x, y, z), rad, mat);
                c++;
            }
            else if (!strcmp(mat, "dielectric")) {
                float ir;
                fscanf(fp, "%f", &ir);
                //printf("%s %f %f %f %s %f\n", type, x, y, z, mat, i);
                material mat = dielectric_create(ir);
                objects[c] = sphere_create(vector_create(x, y, z), rad, mat);
                c++;
            }
            else if (!strcmp(mat, "metal")) {
                float r, g, b, f;
                fscanf(fp, "%f %f %f %f", &r, &g, &b, &f);
                //printf("%s %f %f %f %s %f %f %f %f\n", type, x, y, z, mat, r, g, b, f);
                material mat = metal_create(vector_create(r, g, b), f);
                objects[c] = sphere_create(vector_create(x, y, z), rad, mat);
                c++;
            }
        }
    }

    fclose(fp);
    return (scene) {
        .objects = objects,
        .size = object_count
    };
}

bool scene_hit(scene* s, ray* r, float t_min, float t_max, hit* record) {
    hit temp_record;
    bool hit_anything = false;
    float closest = t_max;
    for (int i = 0; i < s->size; i++) {
        bool object_hit;
        switch (s->objects[i].id) {
            case object_sphere:
                object_hit = sphere_hit(&s->objects[i].data.sphere, r, t_min, closest, &temp_record);
        }
        if (object_hit) {
            hit_anything = true;
            closest = temp_record.t;
            *record = temp_record;
        }
    }
    return hit_anything;
}

void scene_destroy(scene* scn) {
    free(scn->objects);
}
