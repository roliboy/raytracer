#pragma once

#include "material.h"
#include "sphere.h"

typedef enum object_id {
    object_sphere = 0
} object_id;

typedef union object_data {
    sphere sphere;
} object_data;

typedef struct object {
    object_id id;
    object_data data;
} object;

object sphere_create(vec3 center, float radius, material mat) {
    return (object) {
        .id = object_sphere,
        .data = (sphere) {
            .center = center,
            .radius = radius,
            .mat = mat
        }
    };
}
