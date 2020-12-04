#pragma once

#include "object.h"
#include "bounding_box.h"
#include "scene.h"
#include "ray.h"
#include <stdbool.h>

typedef struct bounding_box_node {
    object* left;
    object* right;
    bounding_box bounds;
} bounding_box_node;
/*
bounding_box_node bounding_box_node_create(scene* scn, float time0, float time1) {
    int axis = random_int(0, 2);
    bool (*comparator)(object*, object*);
    if (axis == 0) {
        comparator = &box_x_compare;
    else if (axis == 1)
        comparator = &box_x_compare;
    else if (axis == 2)
        comparator = &box_x_compare;
    return (bounding_box_node) {
        .left = ,
        .right = ,
        .bounds =
    }
}

bvh_node::bvh_node(
    std::vector<shared_ptr<hittable>>& objects,
    size_t start, size_t end, double time0, double time1
) {
    auto objects = src_objects; // Create a modifiable array of the source scene objects

    int axis = random_int(0,2);
    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start+1])) {
            left = objects[start];
            right = objects[start+1];
        } else {
            left = objects[start+1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span/2;
        left = make_shared<bvh_node>(objects, start, mid, time0, time1);
        right = make_shared<bvh_node>(objects, mid, end, time0, time1);
    }

    aabb box_left, box_right;

    if (  !left->bounding_box (time0, time1, box_left)
       || !right->bounding_box(time0, time1, box_right)
    )
        std::cerr << "No bounding box in bvh_node constructor.\n";

    box = surrounding_box(box_left, box_right);
}*/


bool bounding_box_node_hit(bounding_box_node* node, ray* r, float t_min, float t_max, hit* record) {
    if (!bounding_box_hit(&node->bounds, r, t_min, t_max)) return false;

    bool hit_left = object_hit(node->left, r, t_min, t_max, record);
    bool hit_right = object_hit(node->right, r, t_min, hit_left? record->t : t_max, record);

    return hit_left|| hit_right;
}

bool bounding_box_node_bounding_box(bounding_box_node* node, float time0, float time1, bounding_box* box) {
    *box = node->bounds;
    return true;
}

