#include "window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>

#include "framebuffer.h"
#include "scene.h"
#include "vector.h"
// typedef struct window {
//   framebuffer buffer;
//   SDL_Thread *thread;
// } window;

window *window_create(int width, int height, scene *scn) {
  window *created_window = (window *)malloc(sizeof(window));
  created_window->width = width;
  created_window->height = height;
  created_window->buffer = framebuffer_create(scn->width, scn->height);
  created_window->thread =
      SDL_CreateThread(window_thread, "window", (void *)created_window);

  return created_window;
  // // vector widow_size = vector_create(width, height, 0);
  // return (window){
  //     .buffer = framebuffer_create(scn->width, scn->height),
  //     .thread = SDL_CreateThread(window_thread, "window", (void
  //     *)&widow_size)};
}

void window_wait(window *win) { SDL_WaitThread(win->thread, NULL); }

// TODO: refactor
int window_thread(void *_window) {
  window *_win = (window *)_window;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *win = SDL_CreateWindow(
      "i regret my life choices v0.3 reloaded", SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED, _win->width, _win->height, SDL_WINDOW_SHOWN);
  SDL_Renderer *ren =
      SDL_CreateRenderer(win, -1,
                         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC |
                             SDL_RENDERER_TARGETTEXTURE);

  SDL_Texture *offscreen =
      SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET,
                        _win->buffer.width, _win->buffer.height);

  while (1) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        goto cleanup;
      }
    }

    SDL_SetRenderTarget(ren, offscreen);

    pixel *buffer = framebuffer_get(&_win->buffer);
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