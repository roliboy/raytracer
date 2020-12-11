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
    FILE* fp;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    char tag[128];
    int object_count = 0;
    while (fscanf(fp, "%s", tag) == 1) {
        if (!strcmp(tag, "sphere")) object_count++;
        if (!strcmp(tag, "moving_sphere")) object_count++;
    }

    //
    object_count += 1;
    //

    object* objects = (object*)malloc(sizeof(object) * object_count);
    material* materials = (material*)malloc(sizeof(material) * object_count);
    texture* textures = (texture*)malloc(sizeof(texture) * object_count);
    //TODO: get node count
    object* nodes = (object*)malloc(sizeof(object) * object_count * 1.5); //shrug

    rewind(fp);
    int c = 0;
//    sphere 0.000000 -1000.000000 0.000000 1000.000000 diffuse 0.500000 0.500000 0.500000

//    textures[c] = checker_texture_create(vector_create(1, 1, 1), vector_create(0, 0, 0));
    textures[c] = noise_texture_create(4);
    materials[c] = diffuse_create(&textures[c]);
  //  objects[c] = sphere_create(vector_create(0, -1000, 0), 1001.001, &materials[c]);
    objects[c] = sphere_create(vector_create(4, 1, 0), 1.001, &materials[c]);
//sphere 4.000000 1.000000 0.000000 1.000000 metal 0.700000 0.600000 0.500000 0.000000

    //
    c++;
    //

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

/*        if (!strcmp(type, "moving_sphere")) {
            float x0, y0, z0, x1, y1, z1, rad;
            float time0, time1;
            fscanf(fp, "%f %f %f %f %f %f %f %f %f", &x0, &y0, &z0, &x1, &y1, &z1, &time0, &time1, &rad);
            char mat[32];
            fscanf(fp, "%s", mat);
            if (!strcmp(mat, "diffuse")) {
                float r, g, b;
                fscanf(fp, "%f %f %f", &r, &g, &b);
//                printf("diff moving sphere %f %f\n", time0, time1);
                material mat = diffuse_create(texture_solid_color_create(vector_create(r, g, b)));
                objects[c] = moving_sphere_create(vector_create(x0, y0, z0), vector_create(x1, y1, z1), time0, time1, rad, mat);
                c++;
            }
            else if (!strcmp(mat, "dielectric")) {
                float ir;
                fscanf(fp, "%f", &ir);
                //printf("%s %f %f %f %s %f\n", type, x, y, z, mat, i);
                material mat = dielectric_create(ir);
                objects[c] = moving_sphere_create(vector_create(x0, y0, z0), vector_create(x1, y1, z1), time0, time1, rad, mat);
                c++;
            }
            else if (!strcmp(mat, "metal")) {
                float r, g, b, f;
                fscanf(fp, "%f %f %f %f", &r, &g, &b, &f);
                //printf("%s %f %f %f %s %f %f %f %f\n", type, x, y, z, mat, r, g, b, f);
                material mat = metal_create(vector_create(r, g, b), f);
                objects[c] = moving_sphere_create(vector_create(x0, y0, z0), vector_create(x1, y1, z1), time0, time1, rad, mat);
                c++;
            }
        }*/

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
