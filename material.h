#pragma once

#include "materials/dielectric.h"
#include "materials/diffuse.h"
#include "materials/metal.h"
#include "ray.h"
#include "hit.h"
#include "util.h"
#include "vector.h"

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

material diffuse_create(vector albedo) {
    return (material) {
        .id = material_diffuse,
        .data = (material_data) {
            .diffuse = (diffuse) {
                .albedo = albedo
            }
        }
    };
}

material metal_create(vector albedo, float fuzz) {
    return (material) {
        .id = material_metal,
        .data = (material_data) {
            .metal = (metal) {
                .albedo = albedo,
                .fuzz = fuzz
            }
        }
    };
}

material dielectric_create(float ir) {
    return (material) {
        .id = material_dielectric,
        .data = (material_data) {
            .dielectric = (dielectric) {
                .ir = ir
            }
        }
    };
}

bool material_scatter(material* mat, ray* r_in, hit* record, vector* attenuation, ray* r_out) {
    switch (mat->id) {
        case material_diffuse:
            return diffuse_scatter(&mat->data.diffuse, r_in, record, attenuation, r_out);
        case material_metal:
            return metal_scatter(&mat->data.metal, r_in, record, attenuation, r_out);
        case material_dielectric:
            return dielectric_scatter(&mat->data.dielectric, r_in, record, attenuation, r_out);
    }
}
