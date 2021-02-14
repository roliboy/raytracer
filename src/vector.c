#include "vector.h"

#include <math.h>
#include "util.h"

vector vector_create(float x, float y, float z) {
    return (vector) {
        .x = x,
        .y = y,
        .z = z
    };
}


vector vector_add(vector lhs, vector rhs) {
    return (vector) {
        .x = lhs.x + rhs.x,
        .y = lhs.y + rhs.y,
        .z = lhs.z + rhs.z
    };
}

vector vector_subtract(vector lhs, vector rhs) {
    return (vector) {
        .x = lhs.x - rhs.x,
        .y = lhs.y - rhs.y,
        .z = lhs.z - rhs.z
    };
}

vector vector_multiply(vector lhs, vector rhs) {
    return (vector) {
        .x = lhs.x * rhs.x,
        .y = lhs.y * rhs.y,
        .z = lhs.z * rhs.z
    };
}

vector vector_divide(vector lhs, vector rhs) {
    return (vector) {
        .x = lhs.x / rhs.x,
        .y = lhs.y / rhs.y,
        .z = lhs.z / rhs.z
    };
}


vector vector_add_scalar(vector v, float s) {
    return (vector) {
        .x = v.x + s,
        .y = v.y + s,
        .z = v.z + s
    };
}

vector vector_subtract_scalar(vector v, float s) {
    return (vector) {
        .x = v.x - s,
        .y = v.y - s,
        .z = v.z - s
    };
}

vector vector_multiply_scalar(vector v, float s) {
    return (vector) {
        .x = v.x * s,
        .y = v.y * s,
        .z = v.z * s
    };
}

vector vector_divide_scalar(vector v, float s) {
    return (vector) {
        .x = v.x / s,
        .y = v.y / s,
        .z = v.z / s
    };
}


float vector_dot(vector lhs, vector rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

vector vector_cross(vector lhs, vector rhs) {
    return (vector) {
        .x = lhs.y * rhs.z - lhs.z * rhs.y,
        .y = lhs.z * rhs.x - lhs.x * rhs.z,
        .z = lhs.x * rhs.y - lhs.y * rhs.x
    };
}


float vector_norm(vector v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float vector_norm_squared(vector v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}


vector vector_normalize(vector vec) {
    float norm = vector_norm(vec);
    return (vector) {
        .x = vec.x / norm,
        .y = vec.y / norm,
        .z = vec.z / norm
    };
}

vector vector_invert(vector v) {
    return (vector) {
        .x = -v.x,
        .y = -v.y,
        .z = -v.z
    };
}

vector vector_reflect(vector v, vector n) {
    float dot = v.x * n.x + v.y * n.y + v.z * n.z;
    return (vector) {
        .x = v.x - 2 * dot * n.x,
        .y = v.y - 2 * dot * n.y,
        .z = v.z - 2 * dot * n.z
    };
}

vector vector_refract(vector v, vector n, float eta) {
    float cos_theta = fmin(vector_dot(vector_invert(v), n), 1.0);
    vector r_out_perp = vector_multiply_scalar(
            vector_add(
                v,
                vector_multiply_scalar(n, cos_theta)),
            eta);

    vector r_out_parallel = vector_multiply_scalar(n, -sqrt(fabs(1.0 - vector_norm_squared(r_out_perp))));
    return vector_add(r_out_perp, r_out_parallel);
}


vector vector_random_on_unit_sphere() {
    return vector_normalize(vector_create(
                random_float(-1, 1),
                random_float(-1, 1),
                random_float(-1, 1)));
}

vector vector_random_in_unit_sphere() {
    while (true) {
        vector random = vector_create(
                random_float(-1, 1),
                random_float(-1, 1),
                random_float(-1, 1));
        if (vector_norm_squared(random) >= 1) continue;
        return random;
    }
}

vector vector_random_on_unit_disk() {
    return vector_normalize(vector_create(
                random_float(-1, 1),
                random_float(-1, 1),
                0));
}

vector vector_random_in_unit_disk() {
    while (true) {
        vector p = vector_create(
                random_float(-1, 1),
                random_float(-1, 1),
                0);
        if (vector_norm_squared(p) >= 1) continue;
        return p;
    }
}

vector vector_random_in_hemisphere(vector normal) {
    vector in_unit_sphere = vector_random_in_unit_sphere();
//    vector in_unit_sphere = vector_random_on_unit_sphere();
    if (vector_dot(in_unit_sphere, normal) > 0.0)
        return in_unit_sphere;
    else
        return vector_invert(in_unit_sphere);
}

vector vector_random_in_range(float min, float max) {
    return (vector) {
        .x = random_float(min, max),
        .y = random_float(min, max),
        .z = random_float(min, max)
    };
}

bool vector_near_zero(vector vec) {
    const float s = 1e-8;
    return (fabs(vec.x) < s) && (fabs(vec.y) < s) && (fabs(vec.z) < s);
}

vector vector_rgb(vector vec, int samples) {
    float scale = 1.0 / samples;
    float r = sqrt(scale * vec.x);
    float g = sqrt(scale * vec.y);
    float b = sqrt(scale * vec.z);

    return (vector) {
            .x = (float)(int)(256 * clamp(r, 0, 0.999)),
            .y = (float)(int)(256 * clamp(g, 0, 0.999)),
            .z = (float)(int)(256 * clamp(b, 0, 0.999))

    };
}
