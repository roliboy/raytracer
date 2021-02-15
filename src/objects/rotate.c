#include "rotate.h"

#include <math.h>

#include "../bounding_box.h"
#include "../hit.h"
#include "../material.h"
#include "../object.h"
#include "../ray.h"
#include "../util.h"
#include "../vector.h"

object rotate_y_create(object *inner, float angle) {
  rotate_y rotate_y_data;
  rotate_y_data.inner = inner;
  rotate_y_data.sin_theta = sin(angle * M_PI / 180.0);
  rotate_y_data.cos_theta = cos(angle * M_PI / 180.0);

  rotate_y_data.hasbox = object_bounding_box(inner, 0, 1, &rotate_y_data.box);

  vector min = vector_create(infinity, infinity, infinity);
  vector max = vector_create(-infinity, -infinity, -infinity);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        float x = i * rotate_y_data.box.maximum.x +
                  (1 - i) * rotate_y_data.box.minimum.x;
        float y = j * rotate_y_data.box.maximum.y +
                  (1 - j) * rotate_y_data.box.minimum.y;
        float z = k * rotate_y_data.box.maximum.z +
                  (1 - k) * rotate_y_data.box.minimum.z;

        float newx = rotate_y_data.cos_theta * x + rotate_y_data.sin_theta * z;
        float newz = -rotate_y_data.sin_theta * x + rotate_y_data.cos_theta * z;

        vector tester = vector_create(newx, y, newz);

        min.x = fmin(min.x, tester.x);
        max.x = fmax(max.x, tester.x);
        min.y = fmin(min.y, tester.y);
        max.y = fmax(max.y, tester.y);
        min.z = fmin(min.z, tester.z);
        max.z = fmax(max.z, tester.z);
      }
    }
  }

  rotate_y_data.box = bounding_box_create(min, max);

  return (object){.id = object_rotate_y,
                  .data = (object_data){.rotate_y = rotate_y_data}};
}

bool rotate_y_hit(rotate_y *ry, ray *r, float t_min, float t_max, hit *record) {
  vector origin = r->origin;
  vector direction = r->direction;

  origin.x = ry->cos_theta * r->origin.x - ry->sin_theta * r->origin.z;
  origin.z = ry->sin_theta * r->origin.x + ry->cos_theta * r->origin.z;

  direction.x = ry->cos_theta * r->direction.x - ry->sin_theta * r->direction.z;
  direction.z = ry->sin_theta * r->direction.x + ry->cos_theta * r->direction.z;

  ray rotated_r = ray_create(origin, direction, r->time);

  if (!object_hit(ry->inner, &rotated_r, t_min, t_max, record))
    return false;

  vector p = record->p;
  vector normal = record->n;

  p.x = ry->cos_theta * record->p.x + ry->sin_theta * record->p.z;
  p.z = -ry->sin_theta * record->p.x + ry->cos_theta * record->p.z;

  normal.x = ry->cos_theta * record->n.x + ry->sin_theta * record->n.z;
  normal.z = -ry->sin_theta * record->n.x + ry->cos_theta * record->n.z;

  record->p = p;
  hit_set_face_normal(record, &rotated_r, normal);

  return true;
}

bool rotate_y_bounding_box(rotate_y *ry, float time0, float time1,
                           bounding_box *box) {
  *box = ry->box;
  return ry->hasbox;
}
