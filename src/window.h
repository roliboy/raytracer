#pragma once

#include "framebuffer.h"

typedef struct SDL_Thread SDL_Thread;
typedef struct scene scene;

typedef struct window {
  int width;
  int height;
  framebuffer buffer;
  SDL_Thread *thread;
} window;

window *window_create(int width, int height, scene *scn);
void window_wait(window *win);
int window_thread(void *win);