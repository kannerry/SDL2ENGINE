#pragma once
#pragma warning (disable:4244)

#ifndef SDL_HJALP_H

#define SDL_HJALP_H
#include "SDL2/SDL.h"

struct WindowContainer {
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    bool alive = true;
};

int sdl_init(const char* title, int xpos, int ypos, int width, int height, WindowContainer& wc) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return EXIT_FAILURE;
    }
    wc.window = SDL_CreateWindow(
        title,
        xpos,
        ypos,
        width,
        height,
        SDL_WINDOW_SHOWN
    );
    wc.renderer = SDL_CreateRenderer(wc.window, -1, 0);
    return 0;
}

void sdl_cleanup(WindowContainer& window_container) {
    SDL_DestroyWindow(window_container.window);
    SDL_DestroyRenderer(window_container.renderer);
    window_container.alive = false;
    SDL_Quit();
}

#endif // SDL_HJALP_H
