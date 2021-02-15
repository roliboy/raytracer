#include "checker_texture.h"

#include <math.h>

#include "../texture.h"

texture checker_texture_create(vector even, vector odd) {
  return (texture){.id = texture_checker_texture,
                   .data = (texture_data){.checker_texture = (checker_texture){
                                              .even = even, .odd = odd}}};
}

vector checker_texture_value(checker_texture *ct, float u, float v, vector p) {
  float sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
  if (sines < 0)
    return ct->odd;
  else
    return ct->even;
}
