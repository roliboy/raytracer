#pragma once

#include <stdio.h>
#include <math.h>
#include "util.h"

typedef struct vec3 {
    float x, y, z;
} vec3;

vec3 vec3_create(float x, float y, float z) {
    return (vec3) {
        .x = x,
        .y = y,
        .z = z
    };
}

vec3 vec3_add(vec3 lhs, vec3 rhs) {
    return (vec3) {
        .x = lhs.x + rhs.x,
        .y = lhs.y + rhs.y,
        .z = lhs.z + rhs.z
    };
}

vec3 vec3_subtract(vec3 lhs, vec3 rhs) {
    return (vec3) {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

float vec3_norm(vec3 vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float vec3_norm2(vec3 vec) {
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

vec3 vec3_normalize(vec3 vec) {
    float norm = vec3_norm(vec);
    return (vec3) {
        vec.x / norm,
        vec.y / norm,
        vec.z / norm
    };
}

vec3 vec3_multiply(vec3 vec, float d) {
    return (vec3) {
        vec.x * d,
        vec.y * d,
        vec.z * d
    };
}

vec3 vec3_divide(vec3 vec, float d) {
    return (vec3) {
        vec.x / d,
        vec.y / d,
        vec.z / d
    };
}

vec3 vec3_multiply_vec3(vec3 u, vec3 v) {
    return (vec3) {
        .x = u.x * v.x,
        .y = u.y * v.y,
        .z = u.z * v.z
    };
}

float vec3_dot(vec3 lhs, vec3 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

vec3 vec3_cross(vec3 lhs, vec3 rhs) {
    return (vec3) {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    };
}

vec3 vec3_invert(vec3 v) {
    return (vec3) {
        .x = -v.x,
        .y = -v.y,
        .z = -v.z
    };
}

vec3 vec3_refract(vec3 uv, vec3 n, float etai_over_etat) {
    float cos_theta = fmin(vec3_dot(vec3_invert(uv), n), 1.0);
    vec3 r_out_perp = vec3_multiply(
            vec3_add(
                uv,
                vec3_multiply(n, cos_theta)),
            etai_over_etat);

    vec3 r_out_parallel = vec3_multiply(n, -sqrt(fabs(1.0 - vec3_norm2(r_out_perp))));
    return vec3_add(r_out_perp, r_out_parallel);
}

vec3 vec3_random_unit_vector() {
    return vec3_normalize(vec3_create(
                random_float_range(-1, 1),
                random_float_range(-1, 1),
                random_float_range(-1, 1)));
}

vec3 vec3_random_in_unit_sphere() {
    while (true) {
        vec3 random = vec3_create(
                random_float_range(-1, 1),
                random_float_range(-1, 1),
                random_float_range(-1, 1)
                );
        if (vec3_norm2(random) >= 1) continue;
        return random;
    }
}

vec3 vec3_random_in_unit_disk() {
    while (true) {
        vec3 p = vec3_create(random_float_range(-1, 1), random_float_range(-1, 1), 0);
        if (vec3_norm2(p) >= 1) continue;
        return p;
    }
}

vec3 vec3_random_in_hemisphere(vec3 normal) {
    vec3 in_unit_sphere = vec3_random_in_unit_sphere();
//    vec3 in_unit_sphere = vec3_random_unit_vector();
    if (vec3_dot(in_unit_sphere, normal) > 0.0)
        return in_unit_sphere;
    else
        return vec3_invert(in_unit_sphere);
}

vec3 vec3_random_in_range(float min, float max) {
    return (vec3) {
        .x = random_float_range(min, max),
        .y = random_float_range(min, max),
        .z = random_float_range(min, max)
    };
}

bool vec3_near_zero(vec3 vec) {
    const float s = 1e-8;
    return (fabs(vec.x) < s) && (fabs(vec.y) < s) && (fabs(vec.z) < s);
}

vec3 vec3_reflect(vec3 v, vec3 n) {
    return vec3_subtract(v, vec3_multiply(n, 2 * vec3_dot(v, n)));
}

vec3 vec3_rgb(vec3 vec, int samples) {
    float scale = 1.0 / samples;
    float r = sqrt(scale * vec.x);
    float g = sqrt(scale * vec.y);
    float b = sqrt(scale * vec.z);

    return (vec3) {
            .x = (float)(int)(256 * clamp(r, 0, 0.999)),
            .y = (float)(int)(256 * clamp(g, 0, 0.999)),
            .z = (float)(int)(256 * clamp(b, 0, 0.999))

    };
}
