#include "renderer.h"

#include <SDL2/SDL_cpuinfo.h>
#include <SDL2/SDL_mutex.h>
#include <SDL2/SDL_thread.h>

#include <stdlib.h>
#include <string.h>

#include "camera.h"
#include "framebuffer.h"
#include "hit.h"
#include "material.h"
#include "ray.h"
#include "scene.h"
#include "util.h"

// TODO: move to separate unit
vector color(ray *r, scene *world, int depth) {
  if (depth <= 0)
    return vector_create(0, 0, 0);

  hit record;
  if (!scene_hit(world, r, 0.001, infinity, &record))
    return world->ambient;

  ray scattered;
  vector attenuation;
  vector emitted = material_emit(record.mat, record.u, record.v, record.p);

  if (!material_scatter(record.mat, r, &record, &attenuation, &scattered))
    return emitted;

  return vector_add(
      emitted,
      vector_multiply(attenuation, color(&scattered, world, depth - 1)));
}

renderer *renderer_create(scene *scn, framebuffer *buffer) {
  renderer *created_renderer = (renderer *)malloc(sizeof(renderer));
  created_renderer->thread_count = SDL_GetCPUCount();

  created_renderer->threads = (SDL_Thread **)malloc(
      sizeof(SDL_Thread *) * created_renderer->thread_count);

  created_renderer->mut = SDL_CreateMutex();

  // TODO: this
  // created_renderer->cam = (camera *)malloc(sizeof(camera));
  created_renderer->scn = scn;
  // *created_renderer->cam = scene_get_camera(scn);
  created_renderer->buffer = buffer;

  // state->cam = (camera *)malloc(sizeof(camera));
  // *state->cam = scene_get_camera(state->scn);

  for (int i = 0; i < created_renderer->thread_count; i++) {
    // TODO: rework
    char *threadname =
        (char *)malloc(sizeof(char) * created_renderer->thread_count);
    strcpy(threadname, "rendererX");
    threadname[6] = '0' + i;
    created_renderer->threads[i] =
        SDL_CreateThread(renderer_thread, threadname, created_renderer);
  }

  return created_renderer;
}

void renderer_wait(renderer *renderers) {
  for (int i = 0; i < renderers->thread_count; i++) {
    SDL_WaitThread(renderers->threads[i], 0);
  }
  SDL_DestroyMutex(renderers->mut);
}

int renderer_thread(void *_state) {
  renderer *ren = (renderer *)_state;
  camera cam = scene_get_camera(ren->scn);

  int *batch;

  // TODO: batch = renderer_getbatch() and check form batch[0] == -1
  // printf("%s starting\n", (char *)state);
  while (renderer_getbatch(ren, &batch)) {
    // printf("%s got batch\n", (char *)state);
    for (int i = 0; batch[i] != -1; i++) {
      int x = batch[i] % ren->scn->width;
      int y = batch[i] / ren->scn->width;
      vector pixel_color = vector_create(0, 0, 0);

      for (int s = 0; s < ren->scn->samples_per_pixel; ++s) {
        float u = ((float)x + random_float(0, 1) - 0.5) / (ren->scn->width - 1);
        float v =
            ((float)y + random_float(0, 1) - 0.5) / (ren->scn->height - 1);
        ray r = camera_get_ray(&cam, u, v);
        pixel_color =
            vector_add(pixel_color, color(&r, ren->scn, ren->scn->max_depth));
      }

      vector rgb = vector_rgb(pixel_color, ren->scn->samples_per_pixel);
      framebuffer_set(ren->buffer, x, ren->scn->height - 1 - y, rgb);
    }
  }
  // printf("%s finished\n", (char *)state);
  return 0;
}

// SDL_mutex *mut;
// DO NOT REMOVE
// removing the following unused variable causes the renderer to segfault
// char futamama;

#define segments 32
bool assined[segments] = {false};

bool renderer_getbatch(renderer *ren, int **batch) {
  // printf("%s locking mutex\n", "threadname");
  // SDL_LockMutex(renderers->mut);
  // pthread_mutex_lock(&mut);
  // printf("%s locked mutex\n", "threadname");
  int index = -1;
  for (int i = 0; i < segments; i++) {
    if (!assined[i]) {
      index = i;
      break;
    }
  }
  // printf("%s unlocking mutex\n", threadname);
  // SDL_UnlockMutex(renderers->mut);
  // pthread_mutex_unlock(&mut);
  // printf("%s unlocked mutex\n", threadname);

  assined[index] = true;
  // printf("%s was assigned batch %d\n", threadname, index);

  if (index == -1)
    return false;
  // printf("%d\n", index);

  int blocksize = ren->buffer->width * ren->buffer->height / segments;

  // 0 -> width * height
  // width * height -> 0

  *batch = (int *)malloc(sizeof(int) * (blocksize + 1));
  for (int i = 0; i < blocksize; i++) {
    (*batch)[i] = (segments - index - 1) * blocksize + i;
  }
  (*batch)[blocksize] = -1;
  // SDL_CreateMutex();
  // SDL_LockMutex(SDL_mutex *mutex);
  // SDL_UnlockMutex(SDL_mutex *mutex);

  return true;
}
