#pragma once

#include "hit.h"
#include "object.h"
#include "sphere.h"

typedef struct scene {
    object* objects;
    int size;
} scene;

scene scene_create(object objects[], int size) {
    return (scene) {
        .objects = objects,
        .size = size
    };
}

bool scene_hit(scene* s, ray* r, float t_min, float t_max, hit* record) {
    hit temp_record;
    bool hit_anything = false;
    float closest = t_max;
    for (int i = 0; i < s->size; i++) {
        //bool (*hit_function)(void*, ray*, float, float, hit*);
        bool object_hit;
        switch (s->objects[i].id) {
            case object_sphere:
                object_hit = sphere_hit(&s->objects[i].data.sphere, r, t_min, closest, &temp_record);
//            case
        }
        if (object_hit) {
//        if (hit_function(&s->objects[i].data, r, t_min, closest, &temp_record)) {
            hit_anything = true;
            closest = temp_record.t;
            //TODO: copy
//            record->t = temp_record.t;
//            record->p = temp_record.p;
//            record->n = temp_record.n;
            *record = temp_record;
        }
    }
    return hit_anything;
}
