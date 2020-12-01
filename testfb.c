#include "framebuffer.h"
#include "vec3.h"
#include <stdlib.h>
#include <stdio.h>


int main() {
    framebuffer fb = framebuffer_create(10, 10);

    framebuffer_set(&fb, 2, 3, vec3_create(1, 2, 3));
    framebuffer_set(&fb, 2, 4, vec3_create(1, 2, 3));

    pixel* dirty = framebuffer_get_dirty(&fb);
    for (int i = 0; dirty[i].x != -1; i++) {
        printf("%d %d: %d %d %d\n", dirty[i].x, dirty[i].y, dirty[i].r, dirty[i].g, dirty[i].b);
    }
    free(dirty);

    framebuffer_set(&fb, 1, 3, vec3_create(1, 2, 3));
    framebuffer_set(&fb, 1, 4, vec3_create(1, 2, 3));

    dirty = framebuffer_get_dirty(&fb);
    for (int i = 0; dirty[i].x != -1; i++) {
        printf("%d %d: %d %d %d\n", dirty[i].x, dirty[i].y, dirty[i].r, dirty[i].g, dirty[i].b);
    }
    free(dirty);

    framebuffer_destroy(&fb);
}
