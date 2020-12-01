#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    SDL_Event event;

    while (1) {
        while ( SDL_PollEvent(&event) ) {
            switch (event.type) {
                case SDL_MOUSEMOTION:
                    printf("Mouse moved by %d,%d to (%d,%d)\n",
                            event.motion.xrel, event.motion.yrel,
                            event.motion.x, event.motion.y);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    printf("Mouse button %d pressed at (%d,%d)\n",
                            event.button.button, event.button.x, event.button.y);
                    break;
                case SDL_QUIT:
                    exit(0);
            }
        }
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        for (int j = 0; j < 100; j++) {
            SDL_SetRenderDrawColor(ren, rand()%255, rand()%255, rand()%255, 255);
            SDL_RenderDrawPoint(ren, rand()%640, rand()%480);
        }
        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
