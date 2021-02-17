// TODO: this
#include "scene.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "allocator.h"
#include "camera.h"
#include "hit.h"
#include "material.h"
#include "materials/diffuse_light.h"
#include "object.h"
#include "objects/box.h"
#include "texture.h"
#include "vector.h"

// TODO: material sharing

// scene _scene_load(char *filename) {
//   int object_count = 2000;

//   object *objects = (object *)allocate(sizeof(object) * object_count);
//   material *materials = (material *)allocate(sizeof(material) *
//   object_count); texture *textures = (texture *)allocate(sizeof(texture) *
//   object_count);
//   // TODO: get node count
//   object *nodes = (object *)allocate(sizeof(object) * object_count * 1.5); //
//   shrug

//   int c = 0;

//   textures[0] = solid_color_create(vector_create(0.48, 0.83, 0.53));
//   textures[1] = solid_color_create(vector_create(7, 7, 7));
//   textures[2] = solid_color_create(vector_create(0.7, 0.3, 0.1));
//   textures[5] = solid_color_create(vector_create(0.2, 0.4, 0.9));
//   textures[6] = solid_color_create(vector_create(1, 1, 1));
//   textures[7] = image_texture_create("images/earth.ppm");
//   textures[8] = noise_texture_create(4);
//   textures[9] = solid_color_create(vector_create(0.73, 0.73, 0.73));
//   materials[0] = diffuse_create(&textures[0]);
//   materials[1] = diffuse_light_create(&textures[1]);
//   materials[2] = diffuse_create(&textures[2]);
//   materials[3] = dielectric_create(1.5);
//   materials[4] = metal_create(vector_create(0.8, 0.8, 0.9), 1.0);
//   materials[5] = diffuse_light_create(&textures[7]);
//   materials[6] = diffuse_create(&textures[8]);
//   materials[7] = diffuse_create(&textures[9]);

//   int boxes_per_side = 20;
//   for (int i = 0; i < boxes_per_side; i++) {
//     for (int j = 0; j < boxes_per_side; j++) {
//       float w = 100.0;
//       float x0 = -1000.0 + i * w;
//       float z0 = -1000.0 + j * w;
//       float y0 = 0.0;
//       float x1 = x0 + w;
//       float y1 = random_float(1, 101);
//       float z1 = z0 + w;

//       objects[c] = box_create(vector_create(x0, y0, z0),
//                               vector_create(x1, y1, z1), &materials[0]);
//       c++;
//     }
//   }

//   objects[c++] = zx_rectangle_create(123, 423, 147, 412, 554, &materials[1]);
//   objects[c++] = moving_sphere_create(vector_create(400, 400, 200),
//                                       vector_create(430, 400, 200), 0, 1, 50,
//                                       &materials[2]);
//   objects[c++] = sphere_create(vector_create(260, 150, 45), 50,
//   &materials[3]); objects[c++] = sphere_create(vector_create(0, 150, 145),
//   50, &materials[4]); objects[c++] = sphere_create(vector_create(360, 150,
//   145), 70, &materials[3]);

//   object *b1 = (object *)allocate(sizeof(object));
//   *b1 = sphere_create(vector_create(360, 150, 145), 70, 0);
//   object *b2 = (object *)allocate(sizeof(object));
//   *b2 = sphere_create(vector_create(0, 0, 0), 5000, 0);

//   objects[c++] = constant_medium_create(b1, 0.2, &textures[5]);
//   objects[c++] = constant_medium_create(b2, 0.0001, &textures[6]);

//   object *tr = (object *)allocate(sizeof(object));
//   *tr = sphere_create(vector_create(400, 200, 400), 100, &materials[5]);
//   objects[c++] = translate_create(tr, vector_create(0, 200, 0));
//   objects[c++] = sphere_create(vector_create(220, 280, 300), 80,
//   &materials[6]);

//   int ns = 1000;
//   for (int j = 0; j < ns; j++) {
//     objects[c++] = sphere_create(vector_add(vector_random_in_range(0, 165),
//                                             vector_create(-100, 270, 395)),
//                                  10, &materials[7]);
//   }

//   object *rot = (object *)allocate(sizeof(object));
//   *rot = box_create(vector_create(-200, -200, -200),
//                     vector_create(200, 200, 200), &materials[5]);

//   objects[c++] = rotate_y_create(rot, 180);

//   object *root = (object*)allocate(sizeof(object));
//   root = node_create(objects, nodes, 0, object_count, 0, 1);

//   return (scene){.objects = objects,
//                  .materials = materials,
//                  .textures = textures,
//                  .nodes = nodes,
//                  .root = root};
// }

scene *scene_create(char *filename) {
  FILE *fp;

  // TODO: make this dynamic
  object *objects[10000];
  int c = 0;

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

  char object_type[32];
  while (fscanf(fp, "%s", object_type) == 1) {
    if (!strcmp(object_type, "sphere")) {
      material *mat = (material *)allocate(sizeof(material));
      object *obj = (object *)allocate(sizeof(object));

      float x, y, z, rad;
      fscanf(fp, "%f %f %f %f", &x, &y, &z, &rad);

      // TODO: move allocate into sphere_create?
      *obj = sphere_create(vector_create(x, y, z), rad, mat);
      objects[c] = obj;
      c++;

      char material_type[32];
      fscanf(fp, "%s", material_type);
      if (!strcmp(material_type, "diffuse")) {
        float r, g, b;
        fscanf(fp, "%f %f %f", &r, &g, &b);
        texture *tex = (texture *)allocate(sizeof(texture));
        *mat = diffuse_create(tex);
        *tex = solid_color_create(vector_create(r, g, b));
      } else if (!strcmp(material_type, "diffuse_light")) {
        float r, g, b;
        fscanf(fp, "%f %f %f", &r, &g, &b);
        texture *tex = (texture *)allocate(sizeof(texture));
        *mat = diffuse_light_create(tex);
        *tex = solid_color_create(vector_create(r, g, b));
      } else if (!strcmp(material_type, "dielectric")) {
        float ir;
        fscanf(fp, "%f", &ir);
        *mat = dielectric_create(ir);
      } else if (!strcmp(material_type, "metal")) {
        float r, g, b, f;
        fscanf(fp, "%f %f %f %f", &r, &g, &b, &f);
        *mat = metal_create(vector_create(r, g, b), f);
      } else if (!strcmp(material_type, "isotropic")) {
        float r, g, b;
        fscanf(fp, "%f %f %f", &r, &g, &b);
        texture *tex = (texture *)allocate(sizeof(texture));
        *mat = isotropic_create(tex);
        *tex = solid_color_create(vector_create(r, g, b));
      }
    }
  }

  fclose(fp);

  loaded_scene->root = node_create(objects, 0, c, 0, 1);

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
