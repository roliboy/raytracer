#include "rectangle.h"

#include <math.h>
#include <stdio.h>

#include "../allocator.h"
#include "../bounding_box.h"
#include "../hit.h"
#include "../material.h"
#include "../object.h"
#include "../ray.h"
#include "../vector.h"

object *xy_rectangle_create(float x0, float x1, float y0, float y1, float k,
                            material *mat) {
  object *new_object = (object *)allocate(sizeof(object));
  *new_object = (object){
      .id = object_xy_rectangle,
      .data = (object_data){
          .xy_rectangle = (xy_rectangle){
              .x0 = x0, .x1 = x1, .y0 = y0, .y1 = y1, .k = k, .mat = mat}}};
  return new_object;
}

bool xy_rectangle_hit(xy_rectangle *rect, ray *r, float t_min, float t_max,
                      hit *record) {
  float t = (rect->k - r->origin.z) / r->direction.z;

  if (t < t_min || t > t_max)
    return false;

  float x = r->origin.x + t * r->direction.x;
  float y = r->origin.y + t * r->direction.y;

  if (x < rect->x0 || x > rect->x1 || y < rect->y0 || y > rect->y1)
    return false;

  record->u = (x - rect->x0) / (rect->x1 - rect->x0);
  record->v = (y - rect->y0) / (rect->y1 - rect->y0);
  record->t = t;

  vector outward_normal = vector_create(0, 0, 1);

  hit_set_face_normal(record, r, outward_normal);
  record->mat = rect->mat;
  record->p = ray_at(r, t);

  return true;
}

bool xy_rectangle_bounding_box(xy_rectangle *rect, float time0, float time1,
                               bounding_box *box) {
  *box =
      bounding_box_create(vector_create(rect->x0, rect->y0, rect->k - 0.0001),
                          vector_create(rect->x1, rect->y1, rect->k + 0.0001));
  return true;
}

object *yz_rectangle_create(float y0, float y1, float z0, float z1, float k,
                            material *mat) {
  object *new_object = (object *)allocate(sizeof(object));
  *new_object = (object){
      .id = object_yz_rectangle,
      .data = (object_data){
          .yz_rectangle = (yz_rectangle){
              .y0 = y0, .y1 = y1, .z0 = z0, .z1 = z1, .k = k, .mat = mat}}};
  return new_object;
}

bool yz_rectangle_hit(yz_rectangle *rect, ray *r, float t_min, float t_max,
                      hit *record) {
  float t = (rect->k - r->origin.x) / r->direction.x;

  if (t < t_min || t > t_max)
    return false;

  float y = r->origin.y + t * r->direction.y;
  float z = r->origin.z + t * r->direction.z;

  if (y < rect->y0 || y > rect->y1 || z < rect->z0 || z > rect->z1)
    return false;

  record->u = (y - rect->y0) / (rect->y1 - rect->y0);
  record->v = (z - rect->z0) / (rect->z1 - rect->z0);
  record->t = t;

  vector outward_normal = vector_create(1, 0, 0);

  hit_set_face_normal(record, r, outward_normal);
  record->mat = rect->mat;
  record->p = ray_at(r, t);

  return true;
}

bool yz_rectangle_bounding_box(yz_rectangle *rect, float time0, float time1,
                               bounding_box *box) {
  *box =
      bounding_box_create(vector_create(rect->k - 0.0001, rect->y0, rect->z0),
                          vector_create(rect->k + 0.0001, rect->y1, rect->z1));
  return true;
}

object *zx_rectangle_create(float z0, float z1, float x0, float x1, float k,
                            material *mat) {
  object *new_object = (object *)allocate(sizeof(object));
  *new_object = (object){
      .id = object_zx_rectangle,
      .data = (object_data){
          .zx_rectangle = (zx_rectangle){
              .z0 = z0, .z1 = z1, .x0 = x0, .x1 = x1, .k = k, .mat = mat}}};
  return new_object;
}

bool zx_rectangle_hit(zx_rectangle *rect, ray *r, float t_min, float t_max,
                      hit *record) {
  float t = (rect->k - r->origin.y) / r->direction.y;

  if (t < t_min || t > t_max)
    return false;

  float z = r->origin.z + t * r->direction.z;
  float x = r->origin.x + t * r->direction.x;

  if (z < rect->z0 || z > rect->z1 || x < rect->x0 || x > rect->x1)
    return false;

  record->u = (x - rect->x0) / (rect->x1 - rect->x0);
  record->v = (z - rect->z0) / (rect->z1 - rect->z0);
  record->t = t;

  vector outward_normal = vector_create(0, 1, 0);

  hit_set_face_normal(record, r, outward_normal);
  record->mat = rect->mat;
  record->p = ray_at(r, t);

  return true;
}

bool zx_rectangle_bounding_box(zx_rectangle *rect, float time0, float time1,
                               bounding_box *box) {
  *box =
      bounding_box_create(vector_create(rect->z0, rect->k - 0.0001, rect->x0),
                          vector_create(rect->z1, rect->k + 0.0001, rect->x1));
  return true;
}
