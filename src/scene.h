#pragma once

#include <stdio.h>
#include <string.h>

#include "vector.h"

typedef struct bounding_box bounding_box;
typedef struct camera camera;
typedef struct object object;
typedef struct hit hit;
typedef struct ray ray;

typedef struct scene {
  int width;
  int height;
  int samples_per_pixel;
  int max_depth;
  vector lookfrom;
  vector lookat;
  vector vup;
  float fov;
  float aperture;
  float dist_to_focus;
  float shutter_open;
  float shutter_close;
  vector ambient;
  object *root;
} scene;

scene *scene_create(char *filename);
camera scene_get_camera(scene *scn);
bool scene_hit(scene *scn, ray *r, float t_min, float t_max, hit *record);
bool scene_bounding_box(scene *scn, float time0, float time1,
                        bounding_box *box);
