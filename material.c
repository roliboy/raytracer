#include "material.h"
#include "vector.h"

material diffuse_create(texture* albedo) {
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

material diffuse_light_create(texture* emit) {
    return (material) {
        .id = material_diffuse_light,
        .data = (material_data) {
            .diffuse_light = (diffuse_light) {
                .emit = emit
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
        case material_diffuse_light:
            return diffuse_light_scatter(&mat->data.diffuse_light, r_in, record, attenuation, r_out);
        case material_isotropic:
            return isotropic_scatter(&mat->data.isotropic, r_in, record, attenuation, r_out);
        default:
            return false;
    }
}

vector material_emit(material* mat, float u, float v, vector p) {
    switch (mat->id) {
        case material_diffuse_light:
            return diffuse_light_emit(&mat->data.diffuse_light, u, v, p);
        default:
            return vector_create(0, 0, 0);
    }
}
