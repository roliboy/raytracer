#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_thread.h>
#include <stdio.h>

#include "framebuffer.h"
#include "camera.h"
#include "object.h"
#include "util.h"
#include "vector.h"
#include "ray.h"
#include "scene.h"
#include "hit.h"

vector color(ray* r, scene* world, int depth) {
    if (depth <= 0)
        return vector_create(0, 0, 0);

    hit record;
    if (!scene_hit(world, r, 0.001, 0x1.fffffep+127f, &record))
        return vector_create(0.70, 0.80, 1);
//        return vector_create(0, 0, 0);

    ray scattered;
    vector attenuation;
    vector emitted = material_emit(record.mat, record.u, record.v, record.p);

    if (!material_scatter(record.mat, r, &record, &attenuation, &scattered))
        return emitted;

    return vector_add(
            emitted,
            vector_multiply(
                attenuation,
                color(&scattered, world, depth - 1)
                )
            );
}

const int window_width = 1280;
const int window_height = 720;

//limit polling rate
int render_thread(void* _fb) {
    framebuffer* fb = (framebuffer*)_fb;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow(
            "i regret my life choices v0.2 new and improved",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            window_width, window_height,
            SDL_WINDOW_SHOWN);
    SDL_Renderer *ren = SDL_CreateRenderer(
            win,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

    //?
    SDL_Texture* offscreen = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, fb->width, fb->height);

    while (1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    //SDL_DestroyRenderer(ren);
                    //SDL_DestroyWindow(win);
                    //SDL_Quit();
                    //exit(0);
                    goto cleanup;
            }
        }

        SDL_SetRenderTarget(ren, offscreen);
        //SDL_RenderClear(ren);

        pixel* buffer = framebuffer_get(fb);
        for (int i = 0; buffer[i].x != -1; i++) {
            SDL_SetRenderDrawColor(ren, buffer[i].r, buffer[i].g, buffer[i].b, 255);
            SDL_RenderDrawPoint(ren, buffer[i].x, buffer[i].y);
        }

        //detach
        SDL_SetRenderTarget(ren, NULL);

        //SDL_RenderClear(ren);
        SDL_RenderCopy(ren, offscreen, NULL, NULL);
        //SDL_RenderPresent(ren);

        SDL_RenderPresent(ren);
    }

    cleanup:
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

int main() {
    float aspect_ratio = 16.0 / 9.0;
//    int image_width = 1920 / 8;
    int image_width = 1920 / 8;
    int image_height = (int)(image_width / aspect_ratio);
//    int samples_per_pixel = 32;
    int samples_per_pixel = 64;
    int max_depth = 128;
//    int max_depth = 128;


    scene scn = scene_load("scenes/book1_cover_overkill.scn");

//    vector lookfrom = vector_create(13, 2, 3);
  //  vector lookat = vector_create(0, 0, 0);
    vector lookfrom = vector_create(478, 278, -600);
    vector lookat = vector_create(278, 278, 0);

//    vector lookfrom = vector_create(278, 278, -800);
//    vector lookat = vector_create(278, 278, 0);

    vector vup = vector_create(0, 1, 0);
    float dist_to_focus = 10;
    float aperture = 0.1;

    camera cam = camera_create(lookfrom, lookat, vup, 30, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    framebuffer fb = framebuffer_create(image_width, image_height);

    SDL_Thread* renderer = SDL_CreateThread(render_thread, "renderer", (void*)&fb);

    #pragma omp parallel for
    for (int y = image_height - 1; y >= 0; y--) {
        for (int x = 0; x < image_width; x++) {
            vector pixel_color = vector_create(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                float u = ((float)x + random_float(0, 1) - 0.5) / (image_width - 1);
                float v = ((float)y + random_float(0, 1) - 0.5) / (image_height - 1);

                ray r = camera_get_ray(&cam, u, v);
                pixel_color = vector_add(pixel_color, color(&r, &scn, max_depth));

            }
            vector rgb = vector_rgb(pixel_color, samples_per_pixel);
            framebuffer_set(&fb, x, image_height - 1 - y, rgb);
        }
    }

    scene_destroy(&scn);
    SDL_WaitThread(renderer, NULL);

    /*
    printf("P3\n%d %d\n%d\n", fb.width, fb.height, 255);
    for (int i = 0; i < fb.height * fb.width; i++) {
        printf("%d %d %d\n", fb.buffer[i].r, fb.buffer[i].g, fb.buffer[i].b);
    }
    */

    framebuffer_destroy(&fb);
}
