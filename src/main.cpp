#include <iostream>
#include "SDL.h"
#include "timer.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 1;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

void log_sdl_error(std::ostream &os, const std::string &msg) {
    os << msg << " [ERROR] " << SDL_GetError() << std::endl;
}


int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        log_sdl_error(std::cout, "SDL_Init");
        return 1;
    }

    auto *win = SDL_CreateWindow("scpterm", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (win == nullptr) {
        log_sdl_error(std::cout, "SDL_CreateWindow");
        SDL_Quit();
        return 1;
    }

    auto *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        log_sdl_error(std::cout, "SDL_CreateRenderer");
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    SDL_Event evt;
    bool quit = false;
    Timer fpsTimer;

    while (!quit) {
        fpsTimer.start();

        while (SDL_PollEvent(&evt)) {
            switch(evt.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }
        // update state

        // render changes ( pass in lag/MS_PER_UPDATE to balance out rendering)
        SDL_SetRenderDrawColor(renderer, rand() % 255, rand() % 255, rand() % 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        int frameTicks = fpsTimer.get_ticks();
        if (frameTicks < SCREEN_TICKS_PER_FRAME) {
            SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
