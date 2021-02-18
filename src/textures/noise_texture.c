#include "noise_texture.h"

#include <math.h>

#include "../allocator.h"
#include "../texture.h"

texture *noise_texture_create(float scale) {
  texture *new_texture = (texture *)allocate(sizeof(texture));
  noise_texture nt;
  nt.scale = scale;
  nt.n = noise_create();

  *new_texture = (texture){.id = texture_noise_texture,
                           .data = (texture_data){.noise_texture = nt}};
  return new_texture;
}

vector noise_texture_value(noise_texture *nt, vector p) {
  return vector_multiply_scalar(
      vector_create(1, 1, 1),
      0.5 *
          (1 + sin(nt->scale * p.z + 10 * noise_get_turbulent(&nt->n, p, 7))));
  return vector_multiply_scalar(
      vector_create(1, 1, 1),
      noise_get_turbulent(&nt->n, vector_multiply_scalar(p, nt->scale), 7));
  return vector_multiply_scalar(
      vector_create(1, 1, 1),
      0.5 * (1.0 +
             noise_get_value(&nt->n, vector_multiply_scalar(p, nt->scale))));
}
