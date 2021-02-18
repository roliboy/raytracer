#include "translate.h"

#include "../allocator.h"
#include "../bounding_box.h"
#include "../hit.h"
#include "../material.h"
#include "../object.h"
#include "../ray.h"
#include "../vector.h"

object *translate_create(object *inner, vector offset) {
  object *new_object = (object *)allocate(sizeof(object));
  *new_object =
      (object){.id = object_translate,
               .data = (object_data){
                   .translate = (translate){.inner = inner, .offset = offset}}};
  return new_object;
}

bool translate_hit(translate *tr, ray *r, float t_min, float t_max,
                   hit *record) {
  ray moved_ray =
      ray_create(vector_subtract(r->origin, tr->offset), r->direction, r->time);
  if (!object_hit(tr->inner, &moved_ray, t_min, t_max, record))
    return false;

  record->p = vector_add(record->p, tr->offset);
  hit_set_face_normal(record, &moved_ray, record->n);
  return true;
}

bool translate_bounding_box(translate *tr, float time0, float time1,
                            bounding_box *box) {
  if (!object_bounding_box(tr->inner, time0, time1, box))
    return false;

  *box = bounding_box_create(vector_add(box->minimum, tr->offset),
                             vector_add(box->maximum, tr->offset));

  return true;
}
