#include "node.h"

#include "../object.h"
#include "../util.h"

object *node_create(object **objects, int start, int end, float time0,
                    float time1) {
  int (*comparator)(const void *, const void *);
  switch (random_int(0, 2)) {
  case 0:
    comparator = &bounding_box_x_compare;
    break;
  case 1:
    comparator = &bounding_box_y_compare;
    break;
  case 2:
    comparator = &bounding_box_z_compare;
    break;
  }
  int object_span = end - start;

  object *left;
  object *right;

  if (object_span == 1) {
    left = objects[start];
    right = objects[start];
  } else if (object_span == 2) {
    if (comparator(&objects[start], &objects[start + 1])) {
      left = objects[start];
      right = objects[start + 1];
    } else {
      left = objects[start + 1];
      right = objects[start];
    }
  } else {
    qsort(&objects[start], object_span, sizeof(object *), comparator);

    int mid = start + object_span / 2;

    left = node_create(objects, start, mid, time0, time1);
    right = node_create(objects, mid, end, time0, time1);
  }

  bounding_box box_left, box_right;

  if (!object_bounding_box(left, time0, time1, &box_left) ||
      !object_bounding_box(right, time0, time1, &box_right)) {
    // puts("thefuck");
    exit(-1);
  }

  bounding_box box = bounding_box_surround(box_left, box_right);

  object *root = (object *)malloc(sizeof(object));

  *root = (object){
      .id = object_node,
      .data = (object_data){
          .node = (node){.left = left, .right = right, .bounds = box}}};

  return root;
}

bool node_hit(node *n, ray *r, float t_min, float t_max, hit *record) {
  // if (!bounding_box_hit3(&n->bounds, r, t_min, t_max)) return false;
  if (!bounding_box_hit_avx2(&n->bounds, r, t_min, t_max))
    return false;

  bool hit_left = object_hit(n->left, r, t_min, t_max, record);
  bool hit_right =
      object_hit(n->right, r, t_min, hit_left ? record->t : t_max, record);

  return hit_left || hit_right;
}

bool node_bounding_box(node *n, float time0, float time1, bounding_box *box) {
  *box = n->bounds;
  return true;
}
