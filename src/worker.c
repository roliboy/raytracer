#include "worker.h"

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

worker worker_create(int number, worker_state *state) {
  worker created_workers;
  created_workers.threads =
      (SDL_Thread **)malloc(sizeof(SDL_Thread *) * number);

  state->workers = (worker*)malloc(sizeof(worker));
  *state->workers = created_workers;

  state->cam = (camera*)malloc(sizeof(camera));
  *state->cam = scene_get_camera(state->scn);

  for (int i = 0; i < number; i++) {
    char *threadname = (char *)malloc(sizeof(char) * number);
    strcpy(threadname, "workerX");
    threadname[6] = '0' + i;
    created_workers.threads[i] =
        SDL_CreateThread(worker_thread, threadname, state);
  }

  created_workers.thread_count = number;
  created_workers.mut = SDL_CreateMutex();

  return created_workers;
}

void worker_wait(worker *workers) {
  for (int i = 0; i < workers->thread_count; i++) {
    SDL_WaitThread(workers->threads[i], 0);
  }
  SDL_DestroyMutex(workers->mut);
}

int worker_thread(void *_state) {
  worker_state *state = (worker_state *)_state;

  int *batch;

  // TODO: batch = worker_getbatch() and check form batch[0] == -1
  // printf("%s starting\n", (char *)state);
  while (worker_getbatch(state->workers, state, &batch)) {
    // printf("%s got batch\n", (char *)state);
    for (int i = 0; batch[i] != -1; i++) {
      int x = batch[i] % state->scn->width;
      int y = batch[i] / state->scn->width;
      vector pixel_color = vector_create(0, 0, 0);

      for (int s = 0; s < state->scn->samples_per_pixel; ++s) {
        float u =
            ((float)x + random_float(0, 1) - 0.5) / (state->scn->width - 1);
        float v =
            ((float)y + random_float(0, 1) - 0.5) / (state->scn->height - 1);
        ray r = camera_get_ray(state->cam, u, v);
        pixel_color = vector_add(pixel_color,
                                 color(&r, state->scn, state->scn->max_depth));
      }

      vector rgb = vector_rgb(pixel_color, state->scn->samples_per_pixel);
      framebuffer_set(state->fb, x, state->scn->height - 1 - y, rgb);
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

bool worker_getbatch(worker *workers, worker_state *state, int **batch) {
  // printf("%s locking mutex\n", "threadname");
  // SDL_LockMutex(workers->mut);
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
  // SDL_UnlockMutex(workers->mut);
  // pthread_mutex_unlock(&mut);
  // printf("%s unlocked mutex\n", threadname);

  assined[index] = true;
  // printf("%s was assigned batch %d\n", threadname, index);

  if (index == -1)
    return false;
  // printf("%d\n", index);

  int blocksize = state->fb->width * state->fb->height / segments;

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
