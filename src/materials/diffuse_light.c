#include "diffuse_light.h"

#include "../material.h"
#include "../texture.h"

material diffuse_light_create(texture *emit) {
  return (material){
      .id = material_diffuse_light,
      .data = (material_data){.diffuse_light = (diffuse_light){.emit = emit}}};
}

bool diffuse_light_scatter(diffuse_light *mat, ray *r_in, hit *record,
                           vector *attenuation, ray *r_out) {
  return false;
}

vector diffuse_light_emit(diffuse_light *mat, float u, float v, vector p) {
  return texture_value(mat->emit, u, v, p);
}
