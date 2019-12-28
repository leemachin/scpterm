#include <iostream>
#include <stdio.h>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "timer.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 24;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

enum Scene {
    intro = 0,
    testing,
    LAST_SCENE
};

void log_sdl_error(std::ostream &os, const std::string &msg) {
    os << msg << " [ERROR] " << SDL_GetError() << std::endl;
}

#ifdef _WIN32
const char PATH_SEP = '\\';
#else
const char PATH_SEP = '/';
#endif

SDL_Window* gWin;
SDL_Renderer* gRenderer;
SDL_Texture* gTexture;

int gCurrentScene;

bool init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        log_sdl_error(std::cout, "SDL_Init");
        success = false;
    }

    gWin = SDL_CreateWindow("scpterm", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (gWin == nullptr) {
        log_sdl_error(std::cout, "SDL_CreateWindow");
        success = false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        log_sdl_error(std::cout, "IMG_Init");
        success = false;
    }

    gRenderer = SDL_CreateRenderer(gWin, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        log_sdl_error(std::cout, "SDL_CreateRenderer");
        success = false;
    }

    return success;
}

SDL_Texture* load_texture(std::string path) {
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    SDL_Texture* texture = nullptr;

    if (loadedSurface == nullptr) {
        // TODO: Better logging of errors
        std::cout << "Unable to load image " << path.c_str() << "; SDL_image Error: " << IMG_GetError() << std::endl;
    } else {
        texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }

    return texture;
}

std::string get_resource( const std::string &filename, const std::string &subDir = "") {
    static std::string baseRes;

    if (baseRes.empty()) {
        char* basePath = SDL_GetBasePath();
        if (basePath) {
            baseRes = basePath;
            SDL_free(basePath);
        } else {
            log_sdl_error(std::cout, "scpterm::get_resource_path");
            return "";
        }

        size_t pos = baseRes.rfind("bin");
        baseRes = baseRes.substr(0, pos) + "data" + PATH_SEP;
    }

    return subDir.empty() ? baseRes + filename:  baseRes + subDir + PATH_SEP + filename;
}

void close() {
    SDL_DestroyTexture(gTexture);
    gTexture = nullptr;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;

    SDL_DestroyWindow(gWin);
    gWin = nullptr;

    IMG_Quit();
    SDL_Quit();
}

int main() {
    init();

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
                case SDL_KEYDOWN: {
                    switch (evt.key.keysym.sym) {
                        case SDLK_SPACE:
                            gCurrentScene = (gCurrentScene + 1) % LAST_SCENE;
                            break;
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                    }

                    break;
                }
            }
        }
        // update state

        // render changes ( pass in lag/MS_PER_UPDATE to balance out rendering)
        SDL_RenderClear(gRenderer);

        switch(gCurrentScene) {
            case intro: {
                gTexture = load_texture(get_resource("scplogo.png", "img"));
                if (gTexture == nullptr) {
                    log_sdl_error(std::cout, "scpterm::load_surface");
                    quit = true;
                    break;
                }
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);
                break;
            }
            case testing: {
                SDL_SetRenderDrawColor(gRenderer, 50, 100, 200, SDL_ALPHA_OPAQUE);
                break;
            }

        }

        SDL_RenderPresent(gRenderer);

        int frameTicks = fpsTimer.get_ticks();
        if (frameTicks < SCREEN_TICKS_PER_FRAME) {
            SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
        }
    }

    close();

    return 0;
}
