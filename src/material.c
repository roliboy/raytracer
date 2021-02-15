#include "material.h"

#include "vector.h"

bool material_scatter(material *mat, ray *r_in, hit *record,
                      vector *attenuation, ray *r_out) {
  switch (mat->id) {
  case material_diffuse:
    return diffuse_scatter(&mat->data.diffuse, r_in, record, attenuation,
                           r_out);
  case material_metal:
    return metal_scatter(&mat->data.metal, r_in, record, attenuation, r_out);
  case material_dielectric:
    return dielectric_scatter(&mat->data.dielectric, r_in, record, attenuation,
                              r_out);
  case material_diffuse_light:
    return diffuse_light_scatter(&mat->data.diffuse_light, r_in, record,
                                 attenuation, r_out);
  case material_isotropic:
    return isotropic_scatter(&mat->data.isotropic, r_in, record, attenuation,
                             r_out);
  default:
    return false;
  }
}

vector material_emit(material *mat, float u, float v, vector p) {
  switch (mat->id) {
  case material_diffuse_light:
    return diffuse_light_emit(&mat->data.diffuse_light, u, v, p);
  default:
    return vector_create(0, 0, 0);
  }
}
