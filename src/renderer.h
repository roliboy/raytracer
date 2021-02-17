#pragma once

#include <stdbool.h>

typedef struct framebuffer framebuffer;
typedef struct SDL_Thread SDL_Thread;
typedef struct SDL_mutex SDL_mutex;
typedef struct camera camera;
typedef struct scene scene;

typedef struct renderer {
  int worker_count;
  SDL_Thread **workers;
  SDL_mutex *mutex;
  scene *scene;
  framebuffer *framebuffer;
} renderer;

renderer *renderer_create(scene *scene, framebuffer *framebuffer);
void renderer_wait(renderer *renderer);
int renderer_thread(void *renderer);
bool renderer_getbatch(renderer *renderer, int **batch);