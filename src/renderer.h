#pragma once

#include <stdbool.h>

typedef struct framebuffer framebuffer;
typedef struct SDL_Thread SDL_Thread;
typedef struct SDL_mutex SDL_mutex;
typedef struct camera camera;
typedef struct scene scene;

typedef struct renderer {
  SDL_Thread **threads;
  SDL_mutex *mut;
  scene *scn;
  framebuffer *buffer;
  int thread_count;
} renderer;

// typedef struct renderer_state {
//   scene *scn;
//   camera *cam;
//   framebuffer *fb;
//   renderer *renderers;
// } renderer_state;

renderer *renderer_create(scene *scn, framebuffer *buffer);
void renderer_wait(renderer *renderers);
int renderer_thread(void *state);
bool renderer_getbatch(renderer *renderers, int **batch);