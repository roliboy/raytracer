#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

#include "camera.h"
#include "framebuffer.h"
#include "hit.h"
#include "object.h"
#include "ray.h"
#include "scene.h"
#include "util.h"
#include "vector.h"

vector color(ray *r, scene *world, int depth) {
  if (depth <= 0)
    return vector_create(0, 0, 0);

  hit record;
  if (!scene_hit(world, r, 0.001, infinity, &record))
    return world->ambient;

  ray scattered;
  vector attenuation;
  vector emitted = material_emit(record.mat, record.u, record.v, record.p);

  if (!material_scatter(record.mat, r, &record, &attenuation, &scattered))
    return emitted;

  return vector_add(
      emitted,
      vector_multiply(attenuation, color(&scattered, world, depth - 1)));
}

const int window_width = 1280;
const int window_height = 720;

int render_thread(void *_fb) {
  framebuffer *fb = (framebuffer *)_fb;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *win = SDL_CreateWindow(
      "i regret my life choices v0.3 reloaded", SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
  SDL_Renderer *ren =
      SDL_CreateRenderer(win, -1,
                         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC |
                             SDL_RENDERER_TARGETTEXTURE);

  SDL_Texture *offscreen =
      SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET,
                        fb->width, fb->height);

  while (1) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        goto cleanup;
      }
    }

    SDL_SetRenderTarget(ren, offscreen);

    pixel *buffer = framebuffer_get(fb);
    for (int i = 0; buffer[i].x != -1; i++) {
      SDL_SetRenderDrawColor(ren, buffer[i].r, buffer[i].g, buffer[i].b, 255);
      SDL_RenderDrawPoint(ren, buffer[i].x, buffer[i].y);
    }

    SDL_SetRenderTarget(ren, NULL);
    SDL_RenderCopy(ren, offscreen, NULL, NULL);
    SDL_RenderPresent(ren);
  }

cleanup:
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}

int main() {
  scene scn = scene_load("scenes/test.scn");
  camera cam = scene_get_camera(&scn);
  framebuffer fb = framebuffer_create(scn.width, scn.height);

  SDL_Thread *renderer =
      SDL_CreateThread(render_thread, "renderer", (void *)&fb);

#pragma omp parallel for
  for (int y = scn.height - 1; y >= 0; y--) {
    for (int x = 0; x < scn.width; x++) {
      vector pixel_color = vector_create(0, 0, 0);
      for (int s = 0; s < scn.samples_per_pixel; ++s) {
        float u = ((float)x + random_float(0, 1) - 0.5) / (scn.width - 1);
        float v = ((float)y + random_float(0, 1) - 0.5) / (scn.height - 1);

        ray r = camera_get_ray(&cam, u, v);
        pixel_color = vector_add(pixel_color, color(&r, &scn, scn.max_depth));
      }
      vector rgb = vector_rgb(pixel_color, scn.samples_per_pixel);
      framebuffer_set(&fb, x, scn.height - 1 - y, rgb);
    }
  }

  SDL_WaitThread(renderer, NULL);
}
