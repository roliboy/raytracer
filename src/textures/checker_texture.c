#include "checker_texture.h"

#include <math.h>

#include "../allocator.h"
#include "../texture.h"

texture *checker_texture_create(vector even, vector odd) {
  texture *new_texture = (texture *)allocate(sizeof(texture));
  *new_texture =
      (texture){.id = texture_checker_texture,
                .data = (texture_data){.checker_texture = (checker_texture){
                                           .even = even, .odd = odd}}};
  return new_texture;
}

vector checker_texture_value(checker_texture *ct, float u, float v, vector p) {
  float sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
  if (sines < 0)
    return ct->odd;
  else
    return ct->even;
}
