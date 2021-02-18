#include "metal.h"

#include "../allocator.h"
#include "../hit.h"
#include "../material.h"
#include "../ray.h"

material *metal_create(vector albedo, float fuzz) {
  material *new_material = (material *)allocate(sizeof(material));
  *new_material =
      (material){.id = material_metal,
                 .data = (material_data){
                     .metal = (metal){.albedo = albedo, .fuzz = fuzz}}};
  return new_material;
}

bool metal_scatter(metal *mat, ray *r_in, hit *record, vector *attenuation,
                   ray *r_out) {
  vector reflected =
      vector_reflect(vector_normalize(r_in->direction), record->n);
  *r_out = ray_create(
      record->p,
      vector_add(reflected, vector_multiply_scalar(
                                vector_random_in_unit_sphere(), mat->fuzz)),
      r_in->time);
  *attenuation = mat->albedo;
  return vector_dot(r_out->direction, record->n) > 0;
}
