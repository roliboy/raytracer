#pragma once

#include "materials/dielectric.h"
#include "materials/diffuse.h"
#include "materials/metal.h"
#include "texture.h"
#include "vector.h"
#include "ray.h"

typedef struct hit hit;
typedef enum material_id {
    material_diffuse = 0,
    material_metal = 1,
    material_dielectric = 2
} material_id;

typedef union material_data {
    diffuse diffuse;
    metal metal;
    dielectric dielectric;
} material_data;

typedef struct material {
    material_id id;
    material_data data;
} material;

material diffuse_create(texture* albedo);
//material diffuse_create(texture*);
material metal_create(vector albedo, float fuzz);
material dielectric_create(float ir);
bool material_scatter(material* mat, ray* r_in, hit* record, vector* attenuation, ray* r_out);
