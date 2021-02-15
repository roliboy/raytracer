#include "solid_color.h"

#include "../texture.h"

texture solid_color_create(vector color) {
  return (texture){
      .id = texture_solid_color,
      .data = (texture_data){.solid_color = (solid_color){.color = color}}};
}

vector solid_color_value(solid_color *tsc) { return tsc->color; }
