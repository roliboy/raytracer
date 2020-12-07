#pragma once

#include "bounding_box.h"
#include "hit.h"
#include "material.h"
#include "vector.h"
#include "object.h"

#include <string.h>
#include <stdio.h>

typedef struct scene {
    object* objects;
    object* nodes;
    object root;
} scene;

scene scene_load(char* filename);
bool scene_hit(scene* s, ray* r, float t_min, float t_max, hit* record);
bool scene_bounding_box(scene* scn, float time0, float time1, bounding_box* box);
void scene_destroy(scene* scn);
