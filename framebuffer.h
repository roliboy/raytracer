#pragma once

#include <SDL2/SDL_mutex.h>
#include <stdbool.h>
#include <string.h>
#include "vec3.h"

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
    vec3* buffer;
    bool* dirty;
    SDL_mutex* mutex;
} framebuffer;

framebuffer framebuffer_create(int width, int height) {
    return (framebuffer) {
        .width = width,
        .height = height,
        .buffer = (vec3*)malloc(sizeof(vec3) * width * height),
        .dirty = (bool*)calloc(width * height, sizeof(bool))
    };
}

void framebuffer_set(framebuffer* fb, int x, int y, vec3 color) {
    SDL_LockMutex(fb->mutex);
    fb->buffer[fb->width * y + x] = color;
    fb->dirty[fb->width * y + x] = true;
    SDL_UnlockMutex(fb->mutex);
}

//vec3 frameuffer_get(framebuffer* fb, int x, int y) {
//    return fb->buffer[fb->width * y + x];
//}

pixel* framebuffer_get_dirty(framebuffer* fb) {
    SDL_LockMutex(fb->mutex);
    int dirty_pixel_count = 0;
    for (int i = 0; i < fb->width * fb->height; i++)
        dirty_pixel_count += fb->dirty[i];
    pixel* pixels = (pixel*)malloc(sizeof(pixel) * (dirty_pixel_count + 420));
    for (int i = 0, pi = 0; i < fb->width * fb->height; i++)
        if (fb->dirty[i])
            pixels[pi++] = (pixel) {
                .r = (int)fb->buffer[i].x,
                .g = (int)fb->buffer[i].y,
                .b = (int)fb->buffer[i].z,
                .x = i % fb->width,
                .y = i / fb->width
            };
    memset(fb->dirty, 0, fb->width * fb->height * sizeof(bool));
    pixels[dirty_pixel_count] = (pixel) {
                .r = -1,
                .g = -1,
                .b = -1,
                .x = -1,
                .y = -1
    };
    SDL_UnlockMutex(fb->mutex);
    return pixels;
}

void framebuffer_destroy(framebuffer* fb) {
    SDL_DestroyMutex(fb->mutex);
    free(fb->buffer);
    free(fb->dirty);
}
