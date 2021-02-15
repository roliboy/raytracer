#pragma once

#include "util.h"
#include "ray.h"
#include "vector.h"

typedef struct camera {
    vector origin;
    vector horizontal;
    vector vertical;
    vector lower_left_corner;
    vector u;
    vector v;
    vector w;
    float lens_radius;
    float shutter_open;
    float shutter_close;
} camera;

camera camera_create(vector lookfrom, vector lookat, vector vup, float vfov, float aspect_ratio, float aperture, float focus_dist, float shutter_open, float shutter_close);
ray camera_get_ray(camera* c, float s, float t);
