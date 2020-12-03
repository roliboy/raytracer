#pragma once

#include <SDL2/SDL_mutex.h>
#include <stdbool.h>
#include <string.h>
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

framebuffer framebuffer_create(int width, int height) {
    pixel* buffer = (pixel*)malloc(sizeof(pixel) * (width * height + 1));
    for (int i = 0; i < width * height; i++) {
        buffer[i] = (pixel) {
            .r = 0x20,
            .g = 0x20,
            .b = 0x20,
            .x = i % width,
            .y = i / width
        };
    }
    buffer[width * height] = (pixel) {-1, -1, -1, -1, -1};

    return (framebuffer) {
        .width = width,
        .height = height,
        .buffer = buffer,
    };
}

void framebuffer_set(framebuffer* fb, int x, int y, vector color) {
    fb->buffer[fb->width * y + x] = (pixel) {
        .r = (int)color.x,
        .g = (int)color.y,
        .b = (int)color.z,
        .x = x,
        .y = y
    };
}

pixel* framebuffer_get(framebuffer* fb) {
    return fb->buffer;
}

void framebuffer_destroy(framebuffer* fb) {
    free(fb->buffer);
}
