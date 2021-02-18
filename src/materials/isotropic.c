#include "isotropic.h"

#include <stdbool.h>

#include "../allocator.h"
#include "../hit.h"
#include "../material.h"
#include "../ray.h"
#include "../texture.h"
#include "../vector.h"

material *isotropic_create(texture *albedo) {
  material *new_material = (material *)allocate(sizeof(material));
  *new_material = (material){
      .id = material_isotropic,
      .data = (material_data){.isotropic = (isotropic){.albedo = albedo}}};
  return new_material;
}

bool isotropic_scatter(isotropic *mat, ray *r_in, hit *record,
                       vector *attenuation, ray *r_out) {
  *r_out = ray_create(record->p, vector_random_in_unit_sphere(), r_in->time);
  *attenuation = texture_value(mat->albedo, record->u, record->v, record->p);
  return true;
}
