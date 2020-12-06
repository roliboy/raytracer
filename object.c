#include "object.h"

#include "material.h"
#include "objects/node.h"
#include "vector.h"
#include "scene.h"
#include "util.h"
#include <stdbool.h>
#include <stdlib.h>

object sphere_create(vector center, float radius, material mat) {
    return (object) {
        .id = object_sphere,
        .data = (object_data) {
            .sphere = (sphere) {
                .center = center,
                .radius = radius,
                .mat = mat
            }
        }
    };
}

object moving_sphere_create(vector center0, vector center1, float time0, float time1, float radius, material mat) {
    return (object) {
        .id = object_moving_sphere,
        .data = (object_data) {
            .moving_sphere = (moving_sphere) {
                .center0 = center0,
                .center1 = center1,
                .time0 = time0,
                .time1 = time1,
                .radius = radius,
                .mat = mat
            }
        }
    };
}

bool object_bounding_box(object* obj, float time0, float time1, bounding_box* box) {
    switch (obj->id) {
        case object_node:
            return node_bounding_box(&obj->data.node, time0, time1, box);
        case object_sphere:
            return sphere_bounding_box(&obj->data.sphere, time0, time1, box);
        case object_moving_sphere:
            return moving_sphere_bounding_box(&obj->data.moving_sphere, time0, time1, box);
        default:
            printf("object id: %d\n", obj->id);
            exit(69);
    }
    return false;
}

object node_create(scene* scn, int start, int end, float time0, float time1) {
    int axis = random_int(0, 2);
    int (*comparator)(const void*, const void*);
    //int (*comparator2)(const void*, const void*);
    if (axis == 0)
        comparator = &bounding_box_x_compare;
    else if (axis == 1)
        comparator = &bounding_box_y_compare;
    else if (axis == 2)
        comparator = &bounding_box_z_compare;
    int object_span = end - start;

    object* left;
    object* right;

    if (object_span == 1) {
        left = &scn->objects[start];
        right = &scn->objects[start];
    }
    else if (object_span == 2) {
        if (comparator(&scn->objects[start], &scn->objects[start + 1])) {
            left = &scn->objects[start];
            right = &scn->objects[start + 1];
        } else {
            left = &scn->objects[start + 1];
            right = &scn->objects[start];
        }
    }
    else {
//        pritf("sorting from %p \n")
        qsort(&scn->objects[start], object_span, sizeof(object), comparator);

        int mid = start + object_span / 2;
        left = (object*)malloc(sizeof(object));
        right = (object*)malloc(sizeof(object));
        *left = node_create(scn, start, mid, time0, time1);
        *right = node_create(scn, mid, end, time0, time1);
    }

    bounding_box box_left, box_right;

    if (!object_bounding_box(left, time0, time1, &box_left) || !object_bounding_box(right, time0, time1, &box_right)) {
        puts("thefuck");
        exit(-1);
    }

    bounding_box box = bounding_box_surround(box_left, box_right);

    return (object) {
        .id = object_node,
        .data = (object_data) {
            .node = (node) {
                .left = left,
                .right = right,
                .bounds = box
            }
        }
    };
}

bool object_hit(object* obj, ray* r, float t_min, float t_max, hit* record) {
    switch (obj->id) {
        case object_node:
            return node_hit(&obj->data.node, r, t_min, t_max, record);
        case object_sphere:
            return sphere_hit(&obj->data.sphere, r, t_min, t_max, record);
        case object_moving_sphere:
            return moving_sphere_hit(&obj->data.moving_sphere, r, t_min, t_max, record);
        default:
            puts("chiar el");
            exit(-1);
    }
}

int bounding_box_x_compare(const void* a, const void* b) {
    bounding_box box_a;
    bounding_box box_b;

    if (!object_bounding_box((object*)a, 0, 0, &box_a) || !object_bounding_box((object*)b, 0, 0, &box_b)) {
        puts("reeex");
        exit(-1);
    }

    return (box_a.minimum.x < box_b.minimum.x)? -1 : 1;
}

int bounding_box_y_compare(const void* a, const void* b) {
    bounding_box box_a;
    bounding_box box_b;

    if (!object_bounding_box((object*)a, 0, 0, &box_a) || !object_bounding_box((object*)b, 0, 0, &box_b)) {
        puts("reeey");
//        printf("%s\n", (object*);
        exit(-1);
    }

    return (box_a.minimum.y < box_b.minimum.y)? -1 : 1;
}

int bounding_box_z_compare(const void* a, const void* b) {
    bounding_box box_a;
    bounding_box box_b;

    if (!object_bounding_box((object*)a, 0, 0, &box_a) || !object_bounding_box((object*)b, 0, 0, &box_b)) {
        puts("reeez");
        exit(-1);
    }

    return (box_a.minimum.z < box_b.minimum.z)? -1 : 1;
}

