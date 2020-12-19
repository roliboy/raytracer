CC = gcc
#CFLAGS  = -g -Wall -Wpedantic -O3 -march=native -fopenmp -pg
CFLAGS  = -Wall -Wpedantic -O2 -march=native -fopenmp -mavx2
LDFLAGS = -lm -lSDL2

OBJECTS = build/bounding_box.o build/camera.o build/dielectric.o build/diffuse.o build/framebuffer.o build/hit.o build/material.o build/metal.o build/moving_sphere.o build/node.o build/object.o build/ray.o build/scene.o build/sphere.o build/util.o build/vector.o build/texture.o build/solid_color.o build/checker_texture.o build/noise.o build/noise_texture.o build/image_texture.o build/diffuse_light.o build/invert_texture.o build/rectangle.o

all: tracer

tracer: $(OBJECTS) build/main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

benchmarks: $(OBJECTS) build/benchmarks.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

build/benchmarks.o: benchmarks.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

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

build/texture.o: texture.c texture.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/solid_color.o: textures/solid_color.c textures/solid_color.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/checker_texture.o: textures/checker_texture.c textures/checker_texture.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/noise.o: noise.c noise.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/noise_texture.o: textures/noise_texture.c textures/noise_texture.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/image_texture.o: textures/image_texture.c textures/image_texture.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/diffuse_light.o: materials/diffuse_light.c materials/diffuse_light.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/invert_texture.o: textures/invert_texture.c textures/invert_texture.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c

build/rectangle.o: objects/rectangle.c objects/rectangle.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -c


.PHONY: clean
clean:
	rm $(OBJECTS)
	rm tracer
