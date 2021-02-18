#include "constant_medium.h"

#include <math.h>
#include <stdio.h>

#include "../allocator.h"
#include "../bounding_box.h"
#include "../hit.h"
#include "../material.h"
#include "../object.h"
#include "../ray.h"
#include "../util.h"
#include "../vector.h"

object *constant_medium_create(object *boundary, float density,
                               texture *phase) {
  // material *phase_function = (material *)allocate(sizeof(material));
  material *phase_function = isotropic_create(phase);
  object *new_object = (object *)allocate(sizeof(object));
  *new_object =
      (object){.id = object_constant_medium,
               .data = (object_data){.constant_medium = (constant_medium){
                                         .boundary = boundary,
                                         .neg_inv_density = -1 / density,
                                         .phase_function = phase_function}}};
  return new_object;
}

bool constant_medium_hit(constant_medium *cm, ray *r, float t_min, float t_max,
                         hit *record) {
  hit rec1, rec2;

  if (!object_hit(cm->boundary, r, -infinity, infinity, &rec1))
    return false;

  if (!object_hit(cm->boundary, r, rec1.t + 0.0001, infinity, &rec2))
    return false;

  if (rec1.t < t_min)
    rec1.t = t_min;
  if (rec2.t > t_max)
    rec2.t = t_max;

  if (rec1.t >= rec2.t)
    return false;

  if (rec1.t < 0)
    rec1.t = 0;

  float ray_length = vector_norm(r->direction);
  float distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
  float hit_distance = cm->neg_inv_density * log(random_float(0, 1));

  if (hit_distance > distance_inside_boundary)
    return false;

  record->t = rec1.t + hit_distance / ray_length;
  record->p = ray_at(r, record->t);

  record->n = vector_create(1, 0, 0);
  record->front_face = true;
  record->mat = cm->phase_function;

  return true;
}

bool constant_medium_bounding_box(constant_medium *cm, float time0, float time1,
                                  bounding_box *box) {
  return object_bounding_box(cm->boundary, time0, time1, box);
}
