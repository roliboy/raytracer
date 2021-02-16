#pragma once

#include <stdbool.h>

typedef struct framebuffer framebuffer;
typedef struct SDL_Thread SDL_Thread;
typedef struct SDL_mutex SDL_mutex;
typedef struct camera camera;
typedef struct scene scene;

typedef struct worker {
  SDL_Thread **threads;
  SDL_mutex *mut;
  int empty;
  int empty2;
  int thread_count;
} worker;

typedef struct worker_state {
  scene *scn;
  camera *cam;
  framebuffer *fb;
  worker *workers;
} worker_state;

worker worker_create(int number, worker_state *state);
void worker_wait(worker *workers);
int worker_thread(void *state);
bool worker_getbatch(worker *workers, worker_state *state, int **batch);