#!/usr/bin/env python

from random import random, uniform

size = lambda width, height: print(f'{int(width)} {int(height)}')
ray = lambda samples, bounces: print(f'{samples} {bounces}')
camera = lambda x, y, z: print(f'{x} {y} {z}')
scene = lambda x, y, z: print(f'{x} {y} {z}')
up = lambda x, y, z: print(f'{x} {y} {z}')
settings = lambda fov, aperture, focus, shutter_open, shutter_close: print(f'{fov} {aperture} {focus} {shutter_open} {shutter_close}')
ambient = lambda r, g, b: print(f'{r} {g} {b}')

solid_color = lambda r, g, b: print(f'solid_color {r} {g} {b}')

diffuse = lambda ti: print(f'diffuse {ti}')
diffuse_light = lambda ti: print(f'diffuse_light {ti}')
dielectric = lambda ir: print(f'dielectric {ir}')

sphere = lambda x, y, z, d, mi: print(f'sphere {x} {y} {z} {d} {mi}')
box = lambda x0, y0, z0, x1, y1, z1, mi: print(f'box {x0} {y0} {z0} {x1} {y1} {z1} {mi}')

# makeobject = lambda obj, mat: print(obj + " " + mat)

size(1920/5, 1080/5)
ray(32, 128)
camera(478, 278, -600)
scene(278, 278, 0)
up(0, 1, 0)
settings(40, 0.1, 10, 0.0, 1.0)
ambient(0.7, 0.8, 1)


solid_color(0.48, 0.83, 0.53)
# solid_color(7, 7, 7)
# solid_color(0.7, 0.3, 0.1)

diffuse(0)
# diffuse_light(1)
# diffuse(2)
dielectric(1.5)
# metal(4)


# //   textures[0] = solid_color_create(vector_create(0.48, 0.83, 0.53));
# //   textures[1] = solid_color_create(vector_create(7, 7, 7));
# //   textures[2] = solid_color_create(vector_create(0.7, 0.3, 0.1));
# //   textures[5] = solid_color_create(vector_create(0.2, 0.4, 0.9));
# //   textures[6] = solid_color_create(vector_create(1, 1, 1));
# //   textures[7] = image_texture_create("images/earth.ppm");
# //   textures[8] = noise_texture_create(4);
# //   textures[9] = solid_color_create(vector_create(0.73, 0.73, 0.73));
# //   materials[0] = diffuse_create(&textures[0]);
# //   materials[1] = diffuse_light_create(&textures[1]);
# //   materials[2] = diffuse_create(&textures[2]);
# //   materials[3] = dielectric_create(1.5);
# //   materials[4] = metal_create(vector_create(0.8, 0.8, 0.9), 1.0);
# //   materials[5] = diffuse_light_create(&textures[7]);
# //   materials[6] = diffuse_create(&textures[8]);
# //   materials[7] = diffuse_create(&textures[9]);
for i in range(0, 20):
    for j in range(0, 20):
        w = 100.0
        x0 = -1000.0 + i * w
        z0 = -1000.0 + j * w
        y0 = 0.0
        x1 = x0 + w
        y1 = uniform(1, 101)
        z1 = z0 + w

        box(x0, y0, z0, x1, y1, z1, 0)

sphere(260, 150, 45, 50, 1)
sphere(0, 150, 145, 50, 1)
sphere(360, 150, 145, 70, 1)

# //   objects[c++] = zx_rectangle_create(123, 423, 147, 412, 554, &materials[1]);
# //   objects[c++] = moving_sphere_create(vector_create(400, 400, 200),
# //                                       vector_create(430, 400, 200), 0, 1, 50,
# //                                       &materials[2]);

# objects[c++] = sphere_create(vector_create(260, 150, 45), 50, &materials[3]);
#   objects[c++] = sphere_create(vector_create(0, 150, 145), 50, &materials[4]);
#   objects[c++] = sphere_create(vector_create(360, 150, 145), 70, &materials[3]);

# //   object *b1 = (object *)allocate(sizeof(object));
# //   *b1 = sphere_create(vector_create(360, 150, 145), 70, 0);
# //   object *b2 = (object *)allocate(sizeof(object));
# //   *b2 = sphere_create(vector_create(0, 0, 0), 5000, 0);

# //   objects[c++] = constant_medium_create(b1, 0.2, &textures[5]);
# //   objects[c++] = constant_medium_create(b2, 0.0001, &textures[6]);

# //   object *tr = (object *)allocate(sizeof(object));
# //   *tr = sphere_create(vector_create(400, 200, 400), 100, &materials[5]);
# //   objects[c++] = translate_create(tr, vector_create(0, 200, 0));
# //   objects[c++] = sphere_create(vector_create(220, 280, 300), 80,
# //   &materials[6]);

# //   int ns = 1000;
# //   for (int j = 0; j < ns; j++) {
# //     objects[c++] = sphere_create(vector_add(vector_random_in_range(0, 165),
# //                                             vector_create(-100, 270, 395)),
# //                                  10, &materials[7]);
# //   }

# //   object *rot = (object *)allocate(sizeof(object));
# //   *rot = box_create(vector_create(-200, -200, -200),
# //                     vector_create(200, 200, 200), &materials[5]);

# //   objects[c++] = rotate_y_create(rot, 180);

# //   object *root = (object*)allocate(sizeof(object));
# //   root = node_create(objects, nodes, 0, object_count, 0, 1);

# //   return (scene){.objects = objects,
# //                  .materials = materials,
# //                  .textures = textures,
# //                  .nodes = nodes,
# //                  .root = root};
# // }
