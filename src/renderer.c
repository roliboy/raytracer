#include "renderer.h"

#include <SDL2/SDL_cpuinfo.h>
#include <SDL2/SDL_thread.h>

#include "allocator.h"
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

renderer *renderer_create(scene *scene, framebuffer *framebuffer) {
  renderer *created_renderer = (renderer *)allocate(sizeof(renderer));
  created_renderer->worker_count = SDL_GetCPUCount();
  created_renderer->workers = (SDL_Thread **)allocate(
      sizeof(SDL_Thread *) * created_renderer->worker_count);
  created_renderer->mutex = SDL_CreateMutex();
  created_renderer->scene = scene;
  created_renderer->framebuffer = framebuffer;

  for (int i = 0; i < created_renderer->worker_count; i++) {
    created_renderer->workers[i] =
        SDL_CreateThread(renderer_thread, "renderer", created_renderer);
  }

  return created_renderer;
}

void renderer_wait(renderer *renderer) {
  for (int i = 0; i < renderer->worker_count; i++) {
    SDL_WaitThread(renderer->workers[i], 0);
  }
  SDL_DestroyMutex(renderer->mutex);
}

int renderer_thread(void *_renderer) {
  renderer *this = (renderer *)_renderer;
  camera cam = scene_get_camera(this->scene);

  int *batch;

  // TODO: batch = renderer_getbatch() and check form batch[0] == -1

  while (renderer_getbatch(this, &batch)) {
    for (int i = 0; batch[i] != -1; i++) {
      int x = batch[i] % this->scene->width;
      int y = batch[i] / this->scene->width;
      vector pixel_color = vector_create(0, 0, 0);

      for (int s = 0; s < this->scene->samples_per_pixel; ++s) {
        float u =
            ((float)x + random_float(0, 1) - 0.5) / (this->scene->width - 1);
        float v =
            ((float)y + random_float(0, 1) - 0.5) / (this->scene->height - 1);
        ray r = camera_get_ray(&cam, u, v);
        pixel_color = vector_add(
            pixel_color, color(&r, this->scene, this->scene->max_depth));
      }

      vector rgb = vector_rgb(pixel_color, this->scene->samples_per_pixel);
      framebuffer_set(this->framebuffer, x, this->scene->height - 1 - y, rgb);
    }
    free(batch);
  }

  return 0;
}

// TODO: this
#define segments 128
bool assined[segments] = {false};

bool renderer_getbatch(renderer *ren, int **batch) {
  SDL_LockMutex(ren->mutex);

  int index = -1;
  for (int i = 0; i < segments; i++) {
    if (!assined[i]) {
      index = i;
      break;
    }
  }

  SDL_UnlockMutex(ren->mutex);

  assined[index] = true;

  if (index == -1)
    return false;

  int blocksize = ren->framebuffer->width * ren->framebuffer->height / segments;

  *batch = (int *)malloc(sizeof(int) * (blocksize + 1));
  for (int i = 0; i < blocksize; i++) {
    (*batch)[blocksize - 1 - i] = (segments - index - 1) * blocksize + i;
  }
  (*batch)[blocksize] = -1;

  return true;
}

//TODO: parallelize
#define xfragments 24
#define yfragments 12
int xindex = -1;
int yindex = yfragments - 1;
bool _renderer_getbatch(renderer *renderer, int **batch) {
  int xblocksize = renderer->framebuffer->width / xfragments;
  int yblocksize = renderer->framebuffer->height / yfragments;
  bool done = false;

  SDL_LockMutex(renderer->mutex);

  xindex++;
  if (xindex >= xfragments) {
    xindex = 0;
    yindex--;
  }
  if (yindex < 0) {
    done = true;
  }

  // xindex++;
  // if (xindex >= xfragments - 1) {
  //   xindex = 0;
  //   yindex++;
  // }
  // if (yindex >= yfragments - 2) {
  //   done = true;
  // }

  if (done) {
    SDL_UnlockMutex(renderer->mutex);
    return false;
  } else {
    // printf("assigned blocks %d %d\n", xindex, yindex);

    *batch = (int *)malloc(sizeof(int) * (xblocksize * yblocksize + 1));
    int i = 0;
    for (int y = yblocksize - 1; y >= 0; y--) {
      for (int x = 0; x < xblocksize; x++) {
        (*batch)[i++] =
            (y + yindex * yblocksize) * renderer->framebuffer->width +
            (x + xindex * xblocksize);
      }
    }
    (*batch)[xblocksize * yblocksize] = -1;

    SDL_UnlockMutex(renderer->mutex);
    return true;
  }
}
