// #include <SDL2/SDL.h>
// #include <SDL2/SDL_cpuinfo.h>
// #include <SDL2/SDL_mutex.h>
// #include <SDL2/SDL_pixels.h>
// #include <SDL2/SDL_render.h>
// #include <SDL2/SDL_thread.h>
// #include <SDL2/SDL_timer.h>
// #include <SDL2/SDL_video.h>

// #include <stdbool.h>
// #include <stdio.h>
// #include <string.h>
// #include <time.h>
// #include <unistd.h>

#include "window.h"
#include "worker.h"
// #include "camera.h"
#include "framebuffer.h"
// #include "hit.h"
// #include "object.h"
// #include "ray.h"
#include "scene.h"
// #include "util.h"
// #include "vector.h"

const int window_width = 1280;
const int window_height = 720;

int main() {

  scene scn = scene_load("scenes/book1_cover.scn");
  // framebuffer fb = framebuffer_create(scn.width, scn.height);

  // scn = scene_load("scenes/book1_cover.scn");
  // fb = framebuffer_create(scn.width, scn.height);

  // TODO: move framebuffer to window
  window *win = window_create(window_width, window_height, &scn);
  // SDL_Thread *renderer =
  //     SDL_CreateThread(render_thread, "renderer", (void *)&fb);

  worker_state ws;
  ws.scn = &scn;
  ws.fb = &win->buffer;

  worker workers = worker_create(8, &ws);

  worker_wait(&workers);
  window_wait(win);

  // SDL_WaitThread(renderer, NULL);
}
