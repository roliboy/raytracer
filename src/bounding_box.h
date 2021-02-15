#pragma once

#include "ray.h"
#include "vector.h"

typedef struct bounding_box {
  vector minimum;
  vector maximum;
} bounding_box;

bounding_box bounding_box_create(vector minimum, vector maximum);
bool bounding_box_hit(bounding_box *box, ray *r, float t_min, float t_max);
bool bounding_box_hit2(bounding_box *box, ray *r, float t_min, float t_max);
bool bounding_box_hit3(bounding_box *box, ray *r, float t_min, float t_max);
bool bounding_box_hit_avx(bounding_box *box, ray *r, float t_min, float t_max);
bool bounding_box_hit_avx2(bounding_box *box, ray *r, float t_min, float t_max);
bounding_box bounding_box_surround(bounding_box box0, bounding_box box1);
int bounding_box_x_compare(const void *, const void *);
int bounding_box_y_compare(const void *, const void *);
int bounding_box_z_compare(const void *, const void *);
