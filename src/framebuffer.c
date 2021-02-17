#include "framebuffer.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allocator.h"
#include "vector.h"

framebuffer *framebuffer_create(int width, int height) {
  framebuffer *new_framebuffer = (framebuffer *)allocate(sizeof(framebuffer));
  pixel *buffer = (pixel *)allocate(sizeof(pixel) * (width * height + 1));
  for (int i = 0; i < width * height; i++) {
    buffer[i] = (pixel){
        .r = 0x20, .g = 0x20, .b = 0x20, .x = i % width, .y = i / width};
  }
  buffer[width * height] = (pixel){-1, -1, -1, -1, -1};

  new_framebuffer->width = width;
  new_framebuffer->height = height;
  new_framebuffer->buffer = buffer;

  return new_framebuffer;
}

void framebuffer_set(framebuffer *fb, int x, int y, vector color) {
  fb->buffer[fb->width * y + x] = (pixel){
      .r = (int)color.x, .g = (int)color.y, .b = (int)color.z, .x = x, .y = y};
}

pixel *framebuffer_get(framebuffer *fb) { return fb->buffer; }

// TODO: move to separate module
void framebuffer_save(framebuffer *fb) {
  FILE* file;
  file = fopen("/tmp/render.ppm", "w");


  fprintf(file, "P3\n");
  fprintf(file, "%u %u\n", fb->width, fb->height);
  fprintf(file, "255\n");

  for (int i = 0; fb->buffer[i].x != -1; i++) {
    fprintf(file, "%u %u %u\n", fb->buffer[i].r, fb->buffer[i].g, fb->buffer[i].b);
  }

  fclose(file);

  system("convert /tmp/render.ppm render.png");
}

void framebuffer_destroy(framebuffer *fb) { free(fb->buffer); }
