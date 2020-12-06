#pragma once

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "vector.h"

typedef struct pixel {
    int r;
    int g;
    int b;
    int x;
    int y;
} pixel;

typedef struct framebuffer {
    int width;
    int height;
    pixel* buffer;
} framebuffer;

framebuffer framebuffer_create(int width, int height);
void framebuffer_set(framebuffer* fb, int x, int y, vector color);
pixel* framebuffer_get(framebuffer* fb);
void framebuffer_destroy(framebuffer* fb);
