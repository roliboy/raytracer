#pragma once

typedef struct SDL_Thread SDL_Thread;
typedef struct framebuffer framebuffer;
typedef struct scene scene;

typedef struct window {
  int width;
  int height;
  framebuffer *framebuffer;
  SDL_Thread *worker;
} window;

window *window_create(int window_width, int window_height,
                      int framebuffer_width, int framebuffer_height);
void window_wait(window *window);
int window_thread(void *window);