#include "node.h"

#include "../object.h"

bool node_hit(node* n, ray* r, float t_min, float t_max, hit* record) {
    //if (!bounding_box_hit3(&n->bounds, r, t_min, t_max)) return false;
    if (!bounding_box_hit_avx2(&n->bounds, r, t_min, t_max)) return false;

    bool hit_left = object_hit(n->left, r, t_min, t_max, record);
    bool hit_right = object_hit(n->right, r, t_min, hit_left? record->t : t_max, record);

    return hit_left|| hit_right;
}

bool node_bounding_box(node* n, float time0, float time1, bounding_box* box) {
    *box = n->bounds;
    return true;
}
