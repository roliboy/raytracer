#include "diffuse.h"

#include "../allocator.h"
#include "../hit.h"
#include "../material.h"
#include "../ray.h"

material *diffuse_create(texture *albedo) {
  material *new_material = (material *)allocate(sizeof(material));
  *new_material = (material){
      .id = material_diffuse,
      .data = (material_data){.diffuse = (diffuse){.albedo = albedo}}};
  return new_material;
}

bool diffuse_scatter(diffuse *mat, ray *r_in, hit *record, vector *attenuation,
                     ray *r_out) {
  vector scatter_direction =
      vector_add(record->n, vector_random_on_unit_sphere());

  if (vector_near_zero(scatter_direction)) {
    scatter_direction = record->n;
  }

  *r_out = ray_create(record->p, scatter_direction, r_in->time);
  *attenuation = texture_value(mat->albedo, record->u, record->v, record->p);
  return true;
}
