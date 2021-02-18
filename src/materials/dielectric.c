#include "dielectric.h"

#include <math.h>

#include "../allocator.h"
#include "../hit.h"
#include "../material.h"
#include "../ray.h"
#include "../util.h"
#include "../vector.h"

material *dielectric_create(float ir) {
  material *new_material = (material *)allocate(sizeof(material));
  *new_material =
      (material){.id = material_dielectric,
                 .data = (material_data){.dielectric = (dielectric){.ir = ir}}};
  return new_material;
}

bool dielectric_scatter(dielectric *mat, ray *r_in, hit *record,
                        vector *attenuation, ray *r_out) {
  *attenuation = vector_create(1.0, 1.0, 1.0);
  float refraction_ratio = record->front_face ? (1.0 / mat->ir) : mat->ir;

  vector unit_direction = vector_normalize(r_in->direction);

  double cos_theta =
      fmin(vector_dot(vector_invert(unit_direction), record->n), 1.0);
  double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

  bool cannot_refract = refraction_ratio * sin_theta > 1.0;
  vector direction;

  if (cannot_refract ||
      reflectance(cos_theta, refraction_ratio) > random_float(0, 1))
    direction = vector_reflect(unit_direction, record->n);
  else
    direction = vector_refract(unit_direction, record->n, refraction_ratio);

  *r_out = ray_create(record->p, direction, r_in->time);

  return true;
}
