#pragma once

#include "textures/checker_texture.h"
#include "textures/image_texture.h"
#include "textures/invert_texture.h"
#include "textures/noise_texture.h"
#include "textures/solid_color.h"

typedef struct vector vector;

typedef enum texture_id {
  texture_solid_color = 0,
  texture_checker_texture = 1,
  texture_noise_texture = 2,
  texture_image_texture = 3,
  texture_invert_texture = 4
} texture_id;

typedef union texture_data {
  solid_color solid_color;
  checker_texture checker_texture;
  noise_texture noise_texture;
  image_texture image_texture;
  invert_texture invert_texture;
} texture_data;

typedef struct texture {
  texture_id id;
  texture_data data;
} texture;

vector texture_value(texture *t, float u, float v, vector p);
