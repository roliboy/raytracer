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

camera camera_create(vector lookfrom, vector lookat, vector vup, float vfov, float aspect_ratio, float aperture, float focus_dist, float shutter_open, float shutter_cose) {
    float theta = vfov * M_PI / 180.0;
    float h = tan(theta/2);
    float viewport_height = 2.0 * h;
    float viewport_width = aspect_ratio * viewport_height;

    vector w = vector_normalize(vector_subtract(lookfrom, lookat));
    vector u = vector_normalize(vector_cross(vup, w));
    vector v = vector_cross(w, u);


    vector origin = lookfrom;
    vector horizontal = vector_multiply_scalar(u, focus_dist * viewport_width);
    vector vertical = vector_multiply_scalar(v, focus_dist * viewport_height);
    vector lower_left_corner = vector_subtract(
                origin,
                vector_add(
                    vector_multiply_scalar(w, focus_dist),
                    vector_add(
                        vector_divide_scalar(horizontal, 2),
                        vector_divide_scalar(vertical, 2)
                        )
                    )
                );

    return (camera) {
        .origin = origin,
        .horizontal = horizontal,
        .vertical = vertical,
        .lower_left_corner = lower_left_corner,
        .u = u,
        .v = v,
        .w = w,
        .lens_radius = aperture / 2,
        .shutter_open = shutter_open,
        .shutter_close = shutter_cose
    };
}

ray camera_get_ray(camera* c, float s, float t) {
    vector rd = vector_multiply_scalar(vector_random_in_unit_disk(), c->lens_radius);
    vector offset = vector_add(
            vector_multiply_scalar(c->u, rd.x),
            vector_multiply_scalar(c->v, rd.y)
            );

    return ray_create(
        vector_add(c->origin, offset),
        vector_add(
            c->lower_left_corner,
            vector_add(
                vector_multiply_scalar(c->horizontal, s),
                vector_subtract(
                    vector_multiply_scalar(c->vertical, t),
                    vector_add(c->origin, offset)
                    )
                )
            ),
        random_float(c->shutter_open, c->shutter_close));
}
