#include <SDL2/SDL.h>
#include <SDL2/SDL_mutex.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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

scene scn;      // = scene_load("scenes/book1_cover.scn");
camera cam;     // = scene_get_camera(&scn);
framebuffer fb; // = framebuffer_create(scn.width, scn.height);

SDL_mutex *mut;

// DO NOT REMOVE
// removing the following unused variable causes the renderer to segfault
char futamama;

#define segments 8
bool assined[segments] = {false};

bool getwork(int **batch, char *threadname) {
  printf("%s locking mutex\n", threadname);
  SDL_LockMutex(mut);
  // pthread_mutex_lock(&mut);
  printf("%s locked mutex\n", threadname);
  int index = -1;
  for (int i = 0; i < segments; i++) {
    if (!assined[i]) {
      index = i;
      break;
    }
  }
  printf("%s unlocking mutex\n", threadname);
  SDL_UnlockMutex(mut);
  // pthread_mutex_unlock(&mut);
  printf("%s unlocked mutex\n", threadname);

  assined[index] = true;
  printf("%s was assigned batch %d\n", threadname, index);

  if (index == -1)
    return false;
  // printf("%d\n", index);

  int blocksize = fb.width * fb.height / segments;

  // 0 -> width * height
  // width * height -> 0

  *batch = (int *)malloc(sizeof(int) * (blocksize + 1));
  for (int i = 0; i < blocksize; i++) {
    (*batch)[i] = (segments - index - 1) * blocksize + i;
  }
  (*batch)[blocksize] = -1;
  // SDL_CreateMutex();
  // SDL_LockMutex(SDL_mutex *mutex);
  // SDL_UnlockMutex(SDL_mutex *mutex);

  return true;
}

// void* worker_thread(void *_) {
int worker_thread(void *_) {
  int *batch;

  printf("%s starting\n", (char *)_);
  while (getwork(&batch, _)) {
    printf("%s got batch\n", (char *)_);
    for (int i = 0; batch[i] != -1; i++) {
      int x = batch[i] % scn.width;
      int y = batch[i] / scn.width;
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
  printf("%s finished\n", (char *)_);
  return 0;
}

int main() {
  scn = scene_load("scenes/book1_cover.scn");
  cam = scene_get_camera(&scn);
  fb = framebuffer_create(scn.width, scn.height);
  mut = SDL_CreateMutex();
  // pthread_mutex_init(&mut, NULL);

  SDL_Thread *renderer =
      SDL_CreateThread(render_thread, "renderer", (void *)&fb);

  const int tn = 2;
  SDL_Thread *worker[tn];
  // pthread_t worker[tn];
  for (int i = 0; i < tn; i++) {
    char *threadname = (char *)malloc(sizeof(char) * tn);
    strcpy(threadname, "workerX");
    threadname[6] = '0' + i;
    worker[i] = SDL_CreateThread(worker_thread, threadname, threadname);
    // pthread_create(&(worker[i]), NULL, &worker_thread, threadname);
  }

  for (int i = 0; i < tn; i++) {
    SDL_WaitThread(worker[i], 0);
    // pthread_join(worker[i], NULL);
  }

  SDL_DestroyMutex(mut);
  // pthread_mutex_destroy(&mut);

  SDL_WaitThread(renderer, NULL);
}
