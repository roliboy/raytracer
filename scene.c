#include "scene.h"
#include "hit.h"
#include "material.h"
#include "object.h"
#include "texture.h"
#include "vector.h"

#include <string.h>
#include <stdio.h>
#include <math.h>

scene scene_load(char* filename) {
    int object_count = 2000;

    object* objects = (object*)malloc(sizeof(object) * object_count);
    material* materials = (material*)malloc(sizeof(material) * object_count);
    texture* textures = (texture*)malloc(sizeof(texture) * object_count);
    //TODO: get node count
    object* nodes = (object*)malloc(sizeof(object) * object_count * 1.5); //shrug

    int c = 0;

    textures[0] = solid_color_create(vector_create(0.48, 0.83, 0.53));
    textures[1] = solid_color_create(vector_create(7, 7, 7));
    textures[2] = solid_color_create(vector_create(0.7, 0.3, 0.1));
    textures[5] = solid_color_create(vector_create(0.2, 0.4, 0.9));
    textures[6] = solid_color_create(vector_create(1, 1, 1));
    textures[7] = image_texture_create("images/earth.ppm");
    textures[8] = noise_texture_create(4);
    textures[9] = solid_color_create(vector_create(0.73, 0.73, 0.73));
    materials[0] = diffuse_create(&textures[0]);
    materials[1] = diffuse_light_create(&textures[1]);
    materials[2] = diffuse_create(&textures[2]);
    materials[3] = dielectric_create(1.5);
    materials[4] = metal_create(vector_create(0.8, 0.8, 0.9), 1.0);
    materials[5] = diffuse_light_create(&textures[7]);
    materials[6] = diffuse_create(&textures[8]);
    materials[7] = diffuse_create(&textures[9]);

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            float w = 100.0;
            float x0 = -1000.0 + i*w;
            float z0 = -1000.0 + j*w;
            float y0 = 0.0;
            float x1 = x0 + w;
            float y1 = random_float(1,101);
            float z1 = z0 + w;

            objects[c] = box_create(vector_create(x0, y0, z0), vector_create(x1, y1, z1), &materials[0]);
            c++;
        }
    }


    objects[c++] = zx_rectangle_create(123, 423, 147, 412, 554, &materials[1]);
    objects[c++] = moving_sphere_create(vector_create(400, 400, 200), vector_create(430, 400, 200), 0, 1, 50, &materials[2]);
    objects[c++] = sphere_create(vector_create(260, 150, 45), 50, &materials[3]);
    objects[c++] = sphere_create(vector_create(0, 150, 145), 50, &materials[4]);
    objects[c++] = sphere_create(vector_create(360, 150, 145), 70, &materials[3]);

    object* b1 = (object*)malloc(sizeof(object));
    *b1 = sphere_create(vector_create(360, 150, 145), 70, 0);
    object* b2 = (object*)malloc(sizeof(object));
    *b2 = sphere_create(vector_create(0, 0, 0), 5000, 0);

    objects[c++] = constant_medium_create(b1, 0.2, &textures[5]);
    objects[c++] = constant_medium_create(b2, 0.0001, &textures[6]);

    object* tr = (object*)malloc(sizeof(object));
    *tr = sphere_create(vector_create(400, 200, 400), 100, &materials[5]);
    objects[c++] = translate_create(tr, vector_create(0, 200, 0));
    objects[c++] = sphere_create(vector_create(220, 280, 300), 80, &materials[6]);

    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        objects[c++] = sphere_create(vector_add(vector_random_in_range(0,165), vector_create(-100, 270, 395)), 10, &materials[7]);
    }

    object* root = node_create(objects, nodes, 0, object_count, 0, 1);

    return (scene) {
        .objects = objects,
        .materials = materials,
        .textures = textures,
        .nodes = nodes,
        .root = *root
    };
}

scene _scene_load(char* filename) {
    FILE* fp;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    char tag[128];
    int object_count = 0;
    while (fscanf(fp, "%s", tag) == 1) {
        if (!strcmp(tag, "sphere")) object_count++;
        if (!strcmp(tag, "box")) object_count++;
        if (!strcmp(tag, "moving_sphere")) object_count++;
        if (!strcmp(tag, "constant_medium")) object_count++;
    }

    object* objects = (object*)malloc(sizeof(object) * object_count);
    material* materials = (material*)malloc(sizeof(material) * object_count);
    texture* textures = (texture*)malloc(sizeof(texture) * object_count);
    //TODO: get node count
    object* nodes = (object*)malloc(sizeof(object) * object_count * 1.5); //shrug

    rewind(fp);
    int c = 0;

    char type[32];
    while (fscanf(fp, "%s", type) == 1) {
        if (!strcmp(type, "sphere")) {
            float x, y, z, rad;
            fscanf(fp, "%f %f %f %f", &x, &y, &z, &rad);
            char mat[32];
            fscanf(fp, "%s", mat);
            if (!strcmp(mat, "diffuse")) {
                float r, g, b;
                fscanf(fp, "%f %f %f", &r, &g, &b);
                //printf("%s %f %f %f %s %f %f %f\n", type, x, y, z, mat, r, g, b);
                textures[c] = solid_color_create(vector_create(r, g, b));
                materials[c] = diffuse_create(&textures[c]);
                objects[c] = sphere_create(vector_create(x, y, z), rad, &materials[c]);
                c++;
            }
            else if (!strcmp(mat, "dielectric")) {
                float ir;
                fscanf(fp, "%f", &ir);
                //printf("%s %f %f %f %s %f\n", type, x, y, z, mat, i);
                materials[c] = dielectric_create(ir);
                objects[c] = sphere_create(vector_create(x, y, z), rad, &materials[c]);
                c++;
            }
            else if (!strcmp(mat, "metal")) {
                float r, g, b, f;
                fscanf(fp, "%f %f %f %f", &r, &g, &b, &f);
                //printf("%s %f %f %f %s %f %f %f %f\n", type, x, y, z, mat, r, g, b, f);
                materials[c] = metal_create(vector_create(r, g, b), f);
                objects[c] = sphere_create(vector_create(x, y, z), rad, &materials[c]);
                c++;
            }
        }
    }

    fclose(fp);

    object* root = node_create(objects, nodes, 0, object_count, 0, 1);

    return (scene) {
        .objects = objects,
        .materials = materials,
        .textures = textures,
        .nodes = nodes,
        .root = *root
    };
}

bool scene_hit(scene* s, ray* r, float t_min, float t_max, hit* record) {
    hit temp_record;
    bool hit_anything = false;
    float closest = t_max;
    if (object_hit(&s->root, r, t_min, closest, &temp_record)) {
        hit_anything = true;
        closest = temp_record.t;
        *record = temp_record;
    }
    return hit_anything;
}

bool scene_bounding_box(scene* scn, float time0, float time1, bounding_box* box) {
    bounding_box temp_box;

    if (!object_bounding_box(&scn->root, time0, time1, &temp_box)) return false;
    *box = temp_box;
    return true;
}

void scene_destroy(scene* scn) {
    free(scn->objects);
    free(scn->materials);
    free(scn->textures);
    free(scn->nodes);
}
