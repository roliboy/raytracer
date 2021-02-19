#include "allocator.h"
#include "framebuffer.h"
#include "renderer.h"
#include "scene.h"
#include "window.h"

// TODO: stop renderer threads when window is closed
// TODO: partial renders

int main() {
  allocator_initialize();

  const int window_width = 1280;
  const int window_height = 720;

  scene *scn = scene_create("scenes/book2_cover.scn");
  window *win =
      window_create(window_width, window_height, scn->width, scn->height);
  renderer *ren = renderer_create(scn, win->framebuffer);

  renderer_wait(ren);

  framebuffer_save(win->framebuffer);

  window_wait(win);

  allocator_terminate();
}
