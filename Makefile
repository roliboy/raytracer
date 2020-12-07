CC = gcc
CFLAGS  = -g -Wall -O2 -fopenmp
LDFLAGS = -lm -lSDL2

OBJECTS = build/bounding_box.o build/camera.o build/dielectric.o build/diffuse.o build/framebuffer.o build/hit.o build/main.o build/material.o build/metal.o build/moving_sphere.o build/node.o build/object.o build/ray.o build/scene.o build/sphere.o build/util.o build/vector.o

all: tracer

tracer: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

build/bounding_box.o: bounding_box.c bounding_box.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/camera.o: camera.c camera.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/dielectric.o: materials/dielectric.c materials/dielectric.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/diffuse.o: materials/diffuse.c materials/diffuse.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/framebuffer.o: framebuffer.c framebuffer.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/hit.o: hit.c hit.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/main.o: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/material.o: material.c material.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/metal.o: materials/metal.c materials/metal.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/moving_sphere.o: objects/moving_sphere.c objects/moving_sphere.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/node.o: objects/node.c objects/node.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/object.o: object.c object.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/ray.o: ray.c ray.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/scene.o: scene.c scene.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/sphere.o: objects/sphere.c objects/sphere.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/util.o: util.c util.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/vector.o: vector.c vector.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

.PHONY: clean
clean:
	@rm tracer
