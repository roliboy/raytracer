#include "object.h"

#include <stdio.h>

bool object_bounding_box(object *obj, float time0, float time1,
                         bounding_box *box) {
  switch (obj->id) {
  case object_node:
    return node_bounding_box(&obj->data.node, time0, time1, box);
  case object_sphere:
    return sphere_bounding_box(&obj->data.sphere, time0, time1, box);
  case object_moving_sphere:
    return moving_sphere_bounding_box(&obj->data.moving_sphere, time0, time1,
                                      box);
  case object_xy_rectangle:
    return xy_rectangle_bounding_box(&obj->data.xy_rectangle, time0, time1,
                                     box);
  case object_yz_rectangle:
    return yz_rectangle_bounding_box(&obj->data.yz_rectangle, time0, time1,
                                     box);
  case object_zx_rectangle:
    return zx_rectangle_bounding_box(&obj->data.zx_rectangle, time0, time1,
                                     box);
  case object_box:
    return box_bounding_box(&obj->data.box, time0, time1, box);
  case object_constant_medium:
    return constant_medium_bounding_box(&obj->data.constant_medium, time0,
                                        time1, box);
  case object_translate:
    return translate_bounding_box(&obj->data.translate, time0, time1, box);
  case object_rotate_y:
    return rotate_y_bounding_box(&obj->data.rotate_y, time0, time1, box);
  default:
    printf("object id: %d\n", obj->id);
    exit(69);
  }
  return false;
}

bool object_hit(object *obj, ray *r, float t_min, float t_max, hit *record) {
  switch (obj->id) {
  case object_node:
    return node_hit(&obj->data.node, r, t_min, t_max, record);
  case object_sphere:
    return sphere_hit(&obj->data.sphere, r, t_min, t_max, record);
  case object_moving_sphere:
    return moving_sphere_hit(&obj->data.moving_sphere, r, t_min, t_max, record);
  case object_xy_rectangle:
    return xy_rectangle_hit(&obj->data.xy_rectangle, r, t_min, t_max, record);
  case object_yz_rectangle:
    return yz_rectangle_hit(&obj->data.yz_rectangle, r, t_min, t_max, record);
  case object_zx_rectangle:
    return zx_rectangle_hit(&obj->data.zx_rectangle, r, t_min, t_max, record);
  case object_box:
    return box_hit(&obj->data.box, r, t_min, t_max, record);
  case object_constant_medium:
    return constant_medium_hit(&obj->data.constant_medium, r, t_min, t_max,
                               record);
  case object_translate:
    return translate_hit(&obj->data.translate, r, t_min, t_max, record);
  case object_rotate_y:
    return rotate_y_hit(&obj->data.rotate_y, r, t_min, t_max, record);
  default:
    puts("chiar el");
    exit(-1);
  }
}