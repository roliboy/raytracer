#pragma once

typedef struct _object {
  void *address;
  void (*destructor)(void *);
} _object;

typedef struct allocator {
  _object *objects;
  int object_count;
  int capacity;
} allocator;

extern allocator ga;

void allocator_initialize();
void *allocate(int size);
void *allocate_with_destructor(int size, void (*destructor)(void*));
void deallocate(void *address);
void deallocate_all();
void allocator_terminate();