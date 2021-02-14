#pragma once

#include <stdbool.h>

typedef struct hit hit;
typedef struct ray ray;
typedef struct vector vector;
typedef struct dielectric {
    float ir;
} dielectric;

bool dielectric_scatter(dielectric* mat, ray* r_in, hit* record, vector* attenuation, ray* r_out);
