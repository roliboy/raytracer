#include "moving_sphere.h"

#include <math.h>

#include "../bounding_box.h"
#include "../material.h"
#include "../object.h"
#include "../ray.h"
#include "../vector.h"
#include "../allocator.h"

object *moving_sphere_create(vector center0, vector center1, float time0,
                             float time1, float radius, material *mat) {
  object *new_object = (object *)allocate(sizeof(object));
  *new_object = (object){
      .id = object_moving_sphere,
      .data = (object_data){.moving_sphere = (moving_sphere){.center0 = center0,
                                                             .center1 = center1,
                                                             .time0 = time0,
                                                             .time1 = time1,
                                                             .radius = radius,
                                                             .mat = mat}}};
  return new_object;
}

bool moving_sphere_hit(moving_sphere *s, ray *r, float t_min, float t_max,
                       hit *record) {
  vector center = vector_add(
      s->center0,
      vector_multiply_scalar(vector_subtract(s->center1, s->center0),
                             (r->time - s->time0) / (s->time1 - s->time0)));

  vector oc = vector_subtract(r->origin, center);
  float a = vector_norm_squared(r->direction);
  float half_b = vector_dot(oc, r->direction);
  float c = vector_norm_squared(oc) - s->radius * s->radius;

  float discriminant = half_b * half_b - a * c;

  if (discriminant < 0)
    return false;

  float sqrtd = sqrt(discriminant);

  float root = (-half_b - sqrtd) / a;
  if (root < t_min || t_max < root) {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || t_max < root)
      return false;
  }

  record->t = root;
  record->p = ray_at(r, record->t);

  vector outward_normal =
      vector_divide_scalar(vector_subtract(record->p, center), s->radius);
  hit_set_face_normal(record, r, outward_normal);
  record->mat = s->mat;

  return true;
}

bool moving_sphere_bounding_box(moving_sphere *s, float time0, float time1,
                                bounding_box *box) {
  vector center0 = vector_add(
      s->center0,
      vector_multiply_scalar(vector_subtract(s->center1, s->center0),
                             (time0 - s->time0) / (s->time1 - s->time0)));

  vector center1 = vector_add(
      s->center0,
      vector_multiply_scalar(vector_subtract(s->center1, s->center0),
                             (time1 - s->time0) / (s->time1 - s->time0)));

  vector radius = vector_create(s->radius, s->radius, s->radius);

  *box = bounding_box_surround(
      bounding_box_create(vector_subtract(center0, radius),
                          vector_add(center0, radius)),
      bounding_box_create(vector_subtract(center1, radius),
                          vector_add(center1, radius)));

  return true;
}
