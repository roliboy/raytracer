#pragma once

#include "vector.h"
#include "textures/solid_color.h"
#include "textures/checker_texture.h"
#include "textures/noise_texture.h"

typedef enum texture_id {
    texture_solid_color = 0,
    texture_checker_texture = 1,
    texture_noise_texture = 2
} texture_id;

typedef union texture_data {
    solid_color solid_color;
    checker_texture checker_texture;
    noise_texture noise_texture;
} texture_data;

typedef struct texture {
    texture_id id;
    texture_data data;
} texture;

texture solid_color_create(vector color);
texture checker_texture_create(vector even, vector odd);
texture noise_texture_create(float scale);
vector texture_value(texture* t, float u, float v, vector p);
