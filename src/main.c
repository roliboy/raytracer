#include "allocator.h"
#include "renderer.h"
#include "scene.h"
#include "window.h"

int main() {
  allocator_initialize();

  const int window_width = 1280;
  const int window_height = 720;

  scene *scn = scene_create("scenes/book1_cover_light.scn");
  window *win =
      window_create(window_width, window_height, scn->width, scn->height);
  renderer *ren = renderer_create(scn, win->framebuffer);

  renderer_wait(ren);
  window_wait(win);

  allocator_terminate();
}
