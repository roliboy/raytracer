#include "window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "allocator.h"
#include "framebuffer.h"
#include "scene.h"
#include "vector.h"

window *window_create(int window_width, int window_height,
                      int framebuffer_width, int framebuffer_height) {
  window *created_window = (window *)allocate(sizeof(window));
  created_window->width = window_width;
  created_window->height = window_height;
  created_window->framebuffer =
      framebuffer_create(framebuffer_width, framebuffer_height);
  created_window->worker =
      SDL_CreateThread(window_thread, "window", (void *)created_window);

  return created_window;
}

void window_wait(window *window) { SDL_WaitThread(window->worker, NULL); }

// TODO: refactor
int window_thread(void *_window) {
  window *this = (window *)_window;

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *win = SDL_CreateWindow(
      "i regret my life choices v0.3 reloaded", SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED, this->width, this->height, SDL_WINDOW_SHOWN);
  SDL_Renderer *ren =
      SDL_CreateRenderer(win, -1,
                         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC |
                             SDL_RENDERER_TARGETTEXTURE);

  SDL_Texture *offscreen_buffer =
      SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET,
                        this->framebuffer->width, this->framebuffer->height);

  while (1) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        SDL_DestroyTexture(offscreen_buffer);
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        exit(0);
      }
    }

    SDL_SetRenderTarget(ren, offscreen_buffer);

    pixel *buffer = framebuffer_get(this->framebuffer);
    for (int i = 0; buffer[i].x != -1; i++) {
      SDL_SetRenderDrawColor(ren, buffer[i].r, buffer[i].g, buffer[i].b, 255);
      SDL_RenderDrawPoint(ren, buffer[i].x, buffer[i].y);
    }

    SDL_SetRenderTarget(ren, NULL);
    SDL_RenderCopy(ren, offscreen_buffer, NULL, NULL);
    SDL_RenderPresent(ren);
  }
}