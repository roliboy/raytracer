#include "image_texture.h"

#include <stdlib.h>

#include "../allocator.h"
#include "../texture.h"
#include "../util.h"

texture *image_texture_create(char *file) {
  texture *new_texture = (texture *)allocate(sizeof(texture));
  unsigned int width;
  unsigned int height;
  unsigned char *bytes = load_ppm(file, &width, &height);
  *new_texture =
      (texture){.id = texture_image_texture,
                .data = (texture_data){
                    .image_texture = (image_texture){
                        .bytes = bytes, .width = width, .height = height}}};
  return new_texture;
}

vector image_texture_value(image_texture *it, float u, float v, vector p) {
  u = clamp(u, 0.0, 1.0);
  v = 1.0 - clamp(v, 0.0, 1.0);

  int i = (int)(u * it->width);
  int j = (int)(v * it->height);

  if (i >= it->width)
    i = it->width - 1;
  if (j >= it->height)
    j = it->height - 1;

  float color_scale = 1.0 / 255.0;
  int pixel_index = j * 3 * it->width + i * 3;

  return vector_create(color_scale * it->bytes[pixel_index + 0],
                       color_scale * it->bytes[pixel_index + 1],
                       color_scale * it->bytes[pixel_index + 2]);
}

void image_texture_free(image_texture *it) { free(it->bytes); }
