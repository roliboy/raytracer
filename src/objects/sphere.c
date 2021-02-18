#include "sphere.h"

#include <math.h>

#include "../allocator.h"
#include "../bounding_box.h"
#include "../material.h"
#include "../object.h"
#include "../ray.h"
#include "../vector.h"

object *sphere_create(vector center, float radius, material *mat) {
  object *new_object = (object *)allocate(sizeof(object));
  *new_object = (object){
      .id = object_sphere,
      .data = (object_data){
          .sphere = (sphere){.center = center, .radius = radius, .mat = mat}}};
  return new_object;
}

bool sphere_hit(sphere *s, ray *r, float t_min, float t_max, hit *record) {
  vector oc = vector_subtract(r->origin, s->center);
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
      vector_divide_scalar(vector_subtract(record->p, s->center), s->radius);
  hit_set_face_normal(record, r, outward_normal);
  sphere_uv(s, outward_normal, &record->u, &record->v);
  record->mat = s->mat;

  return true;
}

bool sphere_bounding_box(sphere *s, float time0, float time1,
                         bounding_box *box) {
  // TODO: use constructor
  box->minimum = vector_subtract(
      s->center, vector_create(s->radius, s->radius, s->radius));
  box->maximum =
      vector_add(s->center, vector_create(s->radius, s->radius, s->radius));
  return true;
}

void sphere_uv(sphere *s, vector p, float *u, float *v) {
  float theta = acos(-p.y);
  float phi = atan2(-p.z, p.x) + M_PI;

  *u = phi / (2 * M_PI);
  *v = theta / M_PI;
}
