tracer:
	gcc bounding_box.c -lm -lSDL2 -O2 -fopenmp -c
	gcc camera.c -lm -lSDL2 -O2 -fopenmp -c
	gcc framebuffer.c -lm -lSDL2 -O2 -fopenmp -c
	gcc hit.c -lm -lSDL2 -O2 -fopenmp -c
	gcc main.c -lm -lSDL2 -O2 -fopenmp -c
	gcc material.c -lm -lSDL2 -O2 -fopenmp -c
	gcc materials/dielectric.c -lm -lSDL2 -O2 -fopenmp -c
	gcc materials/diffuse.c -lm -lSDL2 -O2 -fopenmp -c
	gcc materials/metal.c -lm -lSDL2 -O2 -fopenmp -c
	gcc object.c -lm -lSDL2 -O2 -fopenmp -c
	gcc objects/moving_sphere.c -lm -lSDL2 -O2 -fopenmp -c
	gcc objects/node.c -lm -lSDL2 -O2 -fopenmp -c
	gcc objects/sphere.c -lm -lSDL2 -O2 -fopenmp -c
	gcc ray.c -lm -lSDL2 -O2 -fopenmp -c
	gcc scene.c -lm -lSDL2 -O2 -fopenmp -c
	gcc util.c -lm -lSDL2 -O2 -fopenmp -c
	gcc vector.c -lm -lSDL2 -O2 -fopenmp -c
	gcc *.o -lm -lSDL2 -O2 -fopenmp -o tracer

test:
	gcc bounding_box.c -lm -lSDL2 -O2 -fopenmp -c
	gcc camera.c -lm -lSDL2 -O2 -fopenmp -c
	gcc framebuffer.c -lm -lSDL2 -O2 -fopenmp -c
	gcc hit.c -lm -lSDL2 -O2 -fopenmp -c
	gcc test.c -lm -lSDL2 -O2 -fopenmp -c
	gcc material.c -lm -lSDL2 -O2 -fopenmp -c
	gcc materials/dielectric.c -lm -lSDL2 -O2 -fopenmp -c
	gcc materials/diffuse.c -lm -lSDL2 -O2 -fopenmp -c
	gcc materials/metal.c -lm -lSDL2 -O2 -fopenmp -c
	gcc object.c -lm -lSDL2 -O2 -fopenmp -c
	gcc objects/moving_sphere.c -lm -lSDL2 -O2 -fopenmp -c
	gcc objects/node.c -lm -lSDL2 -O2 -fopenmp -c
	gcc objects/sphere.c -lm -lSDL2 -O2 -fopenmp -c
	gcc ray.c -lm -lSDL2 -O2 -fopenmp -c
	gcc scene.c -lm -lSDL2 -O2 -fopenmp -c
	gcc util.c -lm -lSDL2 -O2 -fopenmp -c
	gcc vector.c -lm -lSDL2 -O2 -fopenmp -c
	gcc *.o -lm -lSDL2 -O2 -fopenmp -o test

clean:
	@rm tracer

.PHONY: clean
