#include "solid_color.h"

#include "../allocator.h"
#include "../texture.h"

texture *solid_color_create(vector color) {
  texture *new_texture = (texture *)allocate(sizeof(texture));
  *new_texture = (texture){
      .id = texture_solid_color,
      .data = (texture_data){.solid_color = (solid_color){.color = color}}};
  return new_texture;
}

vector solid_color_value(solid_color *tsc) { return tsc->color; }
