#include "allocator.h"

#include <stdio.h>
#include <stdlib.h>

allocator ga;

void allocator_initialize() {
  ga.capacity = 32;
  ga.objects = (_object *)malloc(sizeof(_object) * ga.capacity);
  ga.object_count = 0;
}

void *allocate(int size) {
  // printf("current size %d\n", ga.object_count);
  if (ga.object_count == ga.capacity) {
    _object *objects_copy = (_object *)malloc(sizeof(_object) * ga.capacity * 2);
    for (int i = 0; i < ga.capacity; i++) {
      objects_copy[i] = ga.objects[i];
    }
    free(ga.objects);
    ga.objects = objects_copy;
    ga.capacity *= 2;
  }

  void *memory = malloc(size);

  ga.objects[ga.object_count].address = memory;
  ga.objects[ga.object_count].destructor = NULL;
  ga.object_count++;

  return memory;
}

void *allocate_with_destructor(int size, void (*destructor)(void *)) {
  if (ga.object_count == ga.capacity) {
    // printf("capacity %d exceeded, resizing\n", ga.capacity);
    _object *objects_copy = (_object *)malloc(sizeof(_object) * ga.capacity * 2);
    for (int i = 0; i < ga.capacity; i++) {
      objects_copy[i] = ga.objects[i];
    }
    free(ga.objects);
    ga.objects = objects_copy;
    ga.capacity *= 2;
  }

  void *memory = malloc(size);

  ga.objects[ga.object_count].address = memory;
  ga.objects[ga.object_count].destructor = destructor;
  ga.object_count++;

  return memory;
}

// TODO: hashmap lookup for values in array
// not urgent, since this method will rarely be used
// and all objects will be mass-dealocated anyway
void deallocate(void *address) {
  // printf("manual deallocate called\n");
  for (int i = 0; i < ga.object_count; i++) {
    if (ga.objects[i].address == address) {
      if (ga.objects[i].destructor != NULL)
        ga.objects[i].destructor(address);
      ga.object_count--;
      for (int j = i; j < ga.object_count; j++) {
        ga.objects[j] = ga.objects[j + 1];
      }
    }
  }
  free(address);
}

void deallocate_all() {
  for (int i = 0; i < ga.object_count; i++) {
    if (ga.objects[i].destructor != NULL)
      ga.objects[i].destructor(ga.objects[i].address);
    free(ga.objects[i].address);
  }
}

void allocator_terminate() {
  for (int i = 0; i < ga.object_count; i++) {
    if (ga.objects[i].destructor != NULL)
      ga.objects[i].destructor(ga.objects[i].address);
    free(ga.objects[i].address);
  }
  free(ga.objects);
}