#pragma once

#include <stdbool.h>

typedef struct vector {
    float x;
    float y;
    float z;
} vector;


vector vector_create(float x, float y, float z);
vector vector_add(vector lhs, vector rhs);
vector vector_subtract(vector lhs, vector rhs);
vector vector_multiply(vector lhs, vector rhs);
vector vector_divide(vector lhs, vector rhs);
vector vector_add_scalar(vector v, float s);
vector vector_subtract_scalar(vector v, float s);
vector vector_multiply_scalar(vector v, float s);
vector vector_divide_scalar(vector v, float s);
float vector_dot(vector lhs, vector rhs);
vector vector_cross(vector lhs, vector rhs);
float vector_norm(vector v);
float vector_norm_squared(vector v);
vector vector_normalize(vector vec);
vector vector_invert(vector v);
vector vector_reflect(vector v, vector n);
vector vector_refract(vector v, vector n, float eta);
vector vector_random_on_unit_sphere();
vector vector_random_in_unit_sphere();
vector vector_random_on_unit_disk();
vector vector_random_in_unit_disk();
vector vector_random_in_hemisphere(vector normal);
vector vector_random_in_range(float min, float max);
bool vector_near_zero(vector vec);
vector vector_rgb(vector vec, int samples);
