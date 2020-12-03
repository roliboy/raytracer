#pragma once

#include "ray.h"
#include "hit.h"
#include "util.h"
#include "vec3.h"

typedef struct lambertian {
    vec3 albedo;
} lambertian;

typedef struct metal {
    vec3 albedo;
    float fuzz;
} metal;

typedef struct dielectric {
    float ir;
} dielectric;

typedef enum material_id {
    material_lambertian = 0,
    material_metal = 1,
    material_dielectric = 2
} material_id;

typedef union material_data {
    lambertian lambertian;
    metal metal;
    dielectric dielectric;
} material_data;

typedef struct material {
    material_id id;
    material_data data;
} material;

material lambertian_create(vec3 albedo) {
    return (material) {
        .id = material_lambertian,
        .data = (material_data) {
            .lambertian = (lambertian) {
                .albedo = albedo
            }
        }
    };
}

material metal_create(vec3 albedo, float fuzz) {
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

bool lambertian_scatter(lambertian* mat, ray* r_in, hit* record, vec3* attenuation, ray* r_out) {
    vec3 scatter_direction = vec3_add(record->n, vec3_random_unit_vector());

    // Catch degenerate scatter direction
    if (vec3_near_zero(scatter_direction)) {
        scatter_direction = record->n;
    }

    *r_out = ray_create(record->p, scatter_direction);
    *attenuation = mat->albedo;
    return true;
}

bool metal_scatter(metal* mat, ray* r_in, hit* record, vec3* attenuation, ray* r_out) {
    vec3 reflected = vec3_reflect(vec3_normalize(r_in->direction), record->n);
    *r_out = ray_create(record->p, vec3_add(reflected,
                vec3_multiply(vec3_random_in_unit_sphere(), mat->fuzz)));
    *attenuation = mat->albedo;
    return vec3_dot(r_out->direction, record->n) > 0;
}

bool dielectric_scatter(dielectric* mat, ray* r_in, hit* record, vec3* attenuation, ray* r_out) {
    *attenuation = vec3_create(1.0, 1.0, 1.0);
    float refraction_ratio = record->front_face ? (1.0/mat->ir) : mat->ir;

    vec3 unit_direction = vec3_normalize(r_in->direction);

    double cos_theta = fmin(vec3_dot(vec3_invert(unit_direction), record->n), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    vec3 direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float())
        direction = vec3_reflect(unit_direction, record->n);
    else
        direction = vec3_refract(unit_direction, record->n, refraction_ratio);

    *r_out = ray_create(record->p, direction);

    return true;
}

bool material_scatter(material* mat, ray* r_in, hit* record, vec3* attenuation, ray* r_out) {
    //bool (*scatter_function)(void*, ray*, hit*, vec3, ray*);
    switch (mat->id) {
        case material_lambertian:
            return lambertian_scatter(&mat->data.lambertian, r_in, record, attenuation, r_out);
        case material_metal:
            return metal_scatter(&mat->data.metal, r_in, record, attenuation, r_out);
        case material_dielectric:
            return dielectric_scatter(&mat->data.dielectric, r_in, record, attenuation, r_out);
    }
}
