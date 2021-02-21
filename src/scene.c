// TODO: this
#include "scene.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "allocator.h"
#include "camera.h"
#include "hit.h"
#include "material.h"
#include "materials/dielectric.h"
#include "materials/diffuse.h"
#include "materials/diffuse_light.h"
#include "materials/metal.h"
#include "object.h"
#include "objects/box.h"
#include "objects/moving_sphere.h"
#include "objects/rectangle.h"
#include "texture.h"
#include "textures/image_texture.h"
#include "textures/noise_texture.h"
#include "textures/solid_color.h"
#include "vector.h"

scene *scene_create(char *filename) {
  FILE *fp;

  fp = fopen(filename, "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  scene *loaded_scene = (scene *)allocate(sizeof(scene));

  fscanf(fp, "%d %d", &loaded_scene->width, &loaded_scene->height);
  fscanf(fp, "%d %d", &loaded_scene->samples_per_pixel,
         &loaded_scene->max_depth);
  fscanf(fp, "%f %f %f", &loaded_scene->lookfrom.x, &loaded_scene->lookfrom.y,
         &loaded_scene->lookfrom.z);
  fscanf(fp, "%f %f %f", &loaded_scene->lookat.x, &loaded_scene->lookat.y,
         &loaded_scene->lookat.z);
  fscanf(fp, "%f %f %f", &loaded_scene->vup.x, &loaded_scene->vup.y,
         &loaded_scene->vup.z);
  fscanf(fp, "%f %f %f %f %f", &loaded_scene->fov, &loaded_scene->aperture,
         &loaded_scene->dist_to_focus, &loaded_scene->shutter_open,
         &loaded_scene->shutter_close);
  fscanf(fp, "%f %f %f", &loaded_scene->ambient.x, &loaded_scene->ambient.y,
         &loaded_scene->ambient.z);

  // TODO: make this dynamic
  texture *textures[10000];
  material *materials[10000];
  object *objects[10000];
  int tc = 0, mc = 0, oc = 0;

  char object_type[32];
  while (fscanf(fp, "%s", object_type) == 1) {
    if (!strcmp(object_type, "solid_color")) {
      float r, g, b;
      fscanf(fp, "%f %f %f", &r, &g, &b);
      textures[tc++] = solid_color_create(vector_create(r, g, b));
    } else if (!strcmp(object_type, "image")) {
      char path[128];
      fscanf(fp, "%s", path);
      textures[tc++] = image_texture_create(path);
    } else if (!strcmp(object_type, "noise")) {
      float scale;
      fscanf(fp, "%f", &scale);
      textures[tc++] = noise_texture_create(scale);
    } else if (!strcmp(object_type, "diffuse")) {
      int ti;
      fscanf(fp, "%d", &ti);
      materials[mc++] = diffuse_create(textures[ti]);
    } else if (!strcmp(object_type, "diffuse_light")) {
      int ti;
      fscanf(fp, "%d", &ti);
      materials[mc++] = diffuse_light_create(textures[ti]);
    } else if (!strcmp(object_type, "metal")) {
      float r, g, b, f;
      fscanf(fp, "%f %f %f %f", &r, &g, &b, &f);
      materials[mc++] = metal_create(vector_create(r, g, b), f);
    } else if (!strcmp(object_type, "dielectric")) {
      float ir;
      fscanf(fp, "%f", &ir);
      materials[mc++] = dielectric_create(ir);
    } else if (!strcmp(object_type, "isotropic")) {
      int ti;
      fscanf(fp, "%d", &ti);
      materials[mc++] = isotropic_create(textures[ti]);
    } else if (!strcmp(object_type, "sphere")) {
      float x, y, z, r;
      int mi;
      fscanf(fp, "%f %f %f %f %d", &x, &y, &z, &r, &mi);
      objects[oc++] = sphere_create(vector_create(x, y, z), r, materials[mi]);
    } else if (!strcmp(object_type, "box")) {
      float x0, y0, z0, x1, y1, z1;
      int mi;
      fscanf(fp, "%f %f %f %f %f %f %d", &x0, &y0, &z0, &x1, &y1, &z1, &mi);
      objects[oc++] = box_create(vector_create(x0, y0, z0),
                                 vector_create(x1, y1, z1), materials[mi]);
    } else if (!strcmp(object_type, "zx_rectangle")) {
      float z0, z1, x0, x1, k;
      int mi;
      fscanf(fp, "%f %f %f %f %f %d", &z0, &z1, &x0, &x1, &k, &mi);
      objects[oc++] = zx_rectangle_create(z0, z1, x0, x1, k, materials[mi]);
    } else if (!strcmp(object_type, "moving_sphere")) {
      float x0, y0, z0, x1, y1, z1, t0, t1, r;
      int mi;
      fscanf(fp, "%f %f %f %f %f %f %f %f %f %d", &x0, &y0, &z0, &x1, &y1, &z1,
             &t0, &t1, &r, &mi);
      objects[oc++] = moving_sphere_create(vector_create(x0, y0, z0),
                                           vector_create(x1, y1, z1), t0, t1, r,
                                           materials[mi]);
    } else if (!strcmp(object_type, "constant_medium")) {
      float x, y, z, r, d;
      int mi;
      fscanf(fp, "%f %f %f %f %f %d", &x, &y, &z, &r, &d, &mi);
      object *boundary = sphere_create(vector_create(x, y, z), r, NULL);
      objects[oc++] = constant_medium_create(boundary, d, materials[mi]);
    }
  }

  fclose(fp);

  loaded_scene->root = node_create(objects, 0, oc, 0, 1);

  return loaded_scene;
}

camera scene_get_camera(scene *scn) {
  return camera_create(scn->lookfrom, scn->lookat, scn->vup, scn->fov,
                       (float)scn->width / (float)scn->height, scn->aperture,
                       scn->dist_to_focus, scn->shutter_open,
                       scn->shutter_close);
}

bool scene_hit(scene *scn, ray *r, float t_min, float t_max, hit *record) {
  hit temp_record;
  bool hit_anything = false;
  float closest = t_max;
  if (object_hit(scn->root, r, t_min, closest, &temp_record)) {
    hit_anything = true;
    closest = temp_record.t;
    *record = temp_record;
  }
  return hit_anything;
}

bool scene_bounding_box(scene *scn, float time0, float time1,
                        bounding_box *box) {
  bounding_box temp_box;

  if (!object_bounding_box(scn->root, time0, time1, &temp_box))
    return false;
  *box = temp_box;
  return true;
}
