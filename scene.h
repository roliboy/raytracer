#pragma once

#include "hit.h"
#include "object.h"
#include "sphere.h"

typedef struct scene {
    object* objects;
    material* materials;
    int size;
} scene;

scene scene_create(object* objects, material* materials, int size) {
    return (scene) {
        .objects = objects,
        .materials = materials,
        .size = size
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
    free(scn->materials);
}
