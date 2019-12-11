#include <iostream>
#include "SDL.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void log_sdl_error(std::ostream &os, const std::string &msg) {
    os << msg << std::endl;
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        log_sdl_error(std::cout, SDL_GetError());
        return 1;
    }

    auto *win = SDL_CreateWindow("scpterm", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (win == nullptr) {
        log_sdl_error(std::cout, SDL_GetError());
        SDL_Quit();
        return 1;
    }

    auto *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        log_sdl_error(std::cout, SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    SDL_Event evt;
    bool quit = false;

    while (!quit) {
        if (SDL_PollEvent(&evt)) {
            switch(evt.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, rand() % 255, rand() % 255, rand() % 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(32);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
