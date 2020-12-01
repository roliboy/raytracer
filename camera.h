#pragma once

#include "ray.h"
#include "vec3.h"

typedef struct camera {
    vec3 origin;
    vec3 horizontal;
    vec3 vertical;
    vec3 lower_left_corner;
    vec3 u;
    vec3 v;
    vec3 w;
    float lens_radius;
} camera;

camera camera_create(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect_ratio, float aperture, float focus_dist) {
    float theta = vfov * M_PI / 180.0;
    float h = tan(theta/2);
    float viewport_height = 2.0 * h;
    float viewport_width = aspect_ratio * viewport_height;

    vec3 w = vec3_normalize(vec3_subtract(lookfrom, lookat));
    vec3 u = vec3_normalize(vec3_cross(vup, w));
    vec3 v = vec3_cross(w, u);


    vec3 origin = lookfrom;
    vec3 horizontal = vec3_multiply(u, focus_dist * viewport_width);
    vec3 vertical = vec3_multiply(v, focus_dist * viewport_height);
    vec3 lower_left_corner = vec3_subtract(
                origin,
                vec3_add(
                    vec3_multiply(w, focus_dist),
                    vec3_add(
                        vec3_divide(horizontal, 2),
                        vec3_divide(vertical, 2)
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
        .lens_radius = aperture / 2
    };
}

ray camera_get_ray(camera* c, float s, float t) {

    vec3 rd = vec3_multiply(vec3_random_in_unit_disk(), c->lens_radius);
    vec3 offset = vec3_add(
            vec3_multiply(c->u, rd.x),
            vec3_multiply(c->v, rd.y)
            );

    return ray_create(
        vec3_add(c->origin, offset),
        vec3_add(
            c->lower_left_corner,
            vec3_add(
                vec3_multiply(c->horizontal, s),
                vec3_subtract(
                    vec3_multiply(c->vertical, t),
                    vec3_add(c->origin, offset)
                    )
                )
            )
        );
}
