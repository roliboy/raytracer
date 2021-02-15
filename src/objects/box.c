#include "box.h"

#include "../bounding_box.h"
#include "../hit.h"
#include "../material.h"
#include "../object.h"
#include "../ray.h"
#include "../vector.h"
#include "rectangle.h"

object box_create(vector min, vector max, material *mat) {
  return (object){
      .id = object_box,
      .data = (object_data){
          .box = (box){
              .min = min,
              .max = max,
              .front = (xy_rectangle){min.x, max.x, min.y, max.y, min.z, mat},
              .back = (xy_rectangle){min.x, max.x, min.y, max.y, max.z,
                                     mat}, // front
              .right = (yz_rectangle){min.y, max.y, min.z, max.z, min.x,
                                      mat}, // right
              .left = (yz_rectangle){min.y, max.y, min.z, max.z, max.x,
                                     mat}, // left
              .bottom =
                  (zx_rectangle){min.z, max.z, min.x, max.x, min.y, mat}, // bot
              .top =
                  (zx_rectangle){min.z, max.z, min.x, max.x, max.y, mat}, // top
              .mat = mat}}};
}

bool box_hit(box *b, ray *r, float t_min, float t_max, hit *record) {
  bool hit_front = xy_rectangle_hit(&b->front, r, t_min, t_max, record);
  if (hit_front)
    t_max = record->t;
  bool hit_back = xy_rectangle_hit(&b->back, r, t_min, t_max, record);
  if (hit_back)
    t_max = record->t;
  bool hit_left = yz_rectangle_hit(&b->left, r, t_min, t_max, record);
  if (hit_left)
    t_max = record->t;
  bool hit_right = yz_rectangle_hit(&b->right, r, t_min, t_max, record);
  if (hit_right)
    t_max = record->t;
  bool hit_top = zx_rectangle_hit(&b->top, r, t_min, t_max, record);
  if (hit_top)
    t_max = record->t;
  bool hit_bottom = zx_rectangle_hit(&b->bottom, r, t_min, t_max, record);
  if (hit_bottom)
    t_max = record->t;
  return hit_front || hit_back || hit_left || hit_right || hit_top ||
         hit_bottom;
}

bool box_bounding_box(box *b, float time0, float time1, bounding_box *box) {
  *box = bounding_box_create(b->min, b->max);
  return true;
}
