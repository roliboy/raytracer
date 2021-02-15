#pragma once

#include <stdbool.h>
#include <stdlib.h>

#include "objects/box.h"
#include "objects/constant_medium.h"
#include "objects/moving_sphere.h"
#include "objects/node.h"
#include "objects/rectangle.h"
#include "objects/rotate.h"
#include "objects/sphere.h"
#include "objects/translate.h"

typedef enum object_id {
  object_node = 0,
  object_sphere = 1,
  object_moving_sphere = 2,
  object_xy_rectangle = 3,
  object_yz_rectangle = 4,
  object_zx_rectangle = 5,
  object_box = 6,
  object_constant_medium = 7,
  object_translate = 8,
  object_rotate_y = 9
} object_id;

typedef union object_data {
  node node;
  sphere sphere;
  moving_sphere moving_sphere;
  xy_rectangle xy_rectangle;
  yz_rectangle yz_rectangle;
  zx_rectangle zx_rectangle;
  box box;
  constant_medium constant_medium;
  translate translate;
  rotate_y rotate_y;
} object_data;

typedef struct object {
  object_id id;
  object_data data;
} object;

bool object_bounding_box(object *obj, float time0, float time1,
                         bounding_box *box);
bool object_hit(object *obj, ray *r, float t_min, float t_max, hit *record);