#pragma once

#include "../bounding_box.h"
#include "../hit.h"
#include "../material.h"
#include "../ray.h"
#include "../vector.h"

typedef struct object object;
typedef struct sphere {
  vector center;
  float radius;
  material *mat;
} sphere;

object sphere_create(vector center, float radius, material *mat);
bool sphere_hit(sphere *s, ray *r, float t_min, float t_max, hit *record);
bool sphere_bounding_box(sphere *s, float time0, float time1,
                         bounding_box *box);
void sphere_uv(sphere *s, vector p, float *u, float *v);
