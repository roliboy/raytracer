#include "texture.h"

#include "vector.h"

vector texture_value(texture *t, float u, float v, vector p) {
  switch (t->id) {
  case texture_solid_color:
    return solid_color_value(&t->data.solid_color);
  case texture_checker_texture:
    return checker_texture_value(&t->data.checker_texture, u, v, p);
  case texture_noise_texture:
    return noise_texture_value(&t->data.noise_texture, p);
  case texture_image_texture:
    return image_texture_value(&t->data.image_texture, u, v, p);
  case texture_invert_texture:
    return invert_texture_value(&t->data.invert_texture, u, v, p);
  default:
    //   TODO: error
    return vector_create(255, 0, 255);
  }
}
