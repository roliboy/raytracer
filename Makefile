tracer: main.c camera.h framebuffer.h hit.h material.h object.h ray.h scene.h util.h vector.h
	@gcc main.c -lm -lSDL2 -O2 -fopenmp -o tracer

clean:
	@rm tracer

.PHONY: clean
