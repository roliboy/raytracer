#pragma once

#include "materials/dielectric.h"
#include "materials/diffuse.h"
#include "materials/isotropic.h"
#include "materials/metal.h"
#include "materials/diffuse_light.h"
#include "texture.h"
#include "vector.h"
#include "ray.h"

typedef struct hit hit;
typedef enum material_id {
    material_diffuse = 0,
    material_metal = 1,
    material_dielectric = 2,
    material_diffuse_light = 3,
    material_isotropic = 4
} material_id;

typedef union material_data {
    diffuse diffuse;
    metal metal;
    dielectric dielectric;
    diffuse_light diffuse_light;
    isotropic isotropic;
} material_data;

typedef struct material {
    material_id id;
    material_data data;
} material;

material diffuse_create(texture* albedo);
material metal_create(vector albedo, float fuzz);
material dielectric_create(float ir);
material diffuse_light_create(texture* emit);
bool material_scatter(material* mat, ray* r_in, hit* record, vector* attenuation, ray* r_out);
vector material_emit(material* mat, float u, float v, vector p);
