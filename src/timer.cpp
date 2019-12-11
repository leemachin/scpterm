//
// Created by Lee Machin on 11/12/2019.
//

#include "timer.h"

Timer::Timer() {
    start_ticks = 0;
    paused_ticks = 0;
    started = false;
    paused = false;
}

void Timer::start() {
    started = true;
    paused = false;

    start_ticks = SDL_GetTicks();
    paused_ticks = 0;
}

void Timer::pause() {
    if (started && !paused) {
        paused = true;
        paused_ticks = SDL_GetTicks();
        start_ticks = 0;
    }
}

void Timer::stop() {
    started = false;
    paused = false;

    start_ticks = 0;
    paused_ticks = 0;
}

void Timer::unpause() {
    if (started && paused) {
        paused = false;
        start_ticks = SDL_GetTicks() - paused_ticks;
        paused_ticks = 0;
    }
}

Uint32 Timer::get_ticks() {
    Uint32 time = 0;
    if (started) {
        time = paused ?  paused_ticks : SDL_GetTicks() - start_ticks;
    }
    return time;
}

bool Timer::is_started() {
    return started;
}

bool Timer::is_paused() {
    return paused;
}