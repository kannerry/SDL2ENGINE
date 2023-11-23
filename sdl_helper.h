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

int sdl_init(const char* title, int xpos, int ypos, int width, int height, WindowContainer* wc) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return EXIT_FAILURE;
    }
    wc->window = SDL_CreateWindow(
        title,
        xpos,
        ypos,
        width,
        height,
        SDL_WINDOW_SHOWN
    );
    wc->renderer = SDL_CreateRenderer(wc->window, -1, 0);
    return 0;
}

void sdl_cleanup(WindowContainer* wc) {
    SDL_DestroyWindow(wc->window);
    SDL_DestroyRenderer(wc->renderer);
    wc->alive = false;
    SDL_Quit();
}

void sdl_default_process_event(SDL_Event* event, WindowContainer* wc) {
    if (event->type == SDL_QUIT) {
        sdl_cleanup(wc);
    }
}

void sdl_default_render_clear(WindowContainer* wc, SDL_Color clr) {
    SDL_SetRenderDrawColor(wc->renderer, clr.r, clr.g, clr.b, clr.a);
    SDL_RenderClear(wc->renderer);
}

void sdl_default_render_present(WindowContainer* wc) {
    SDL_RenderPresent(wc->renderer);
}

#endif // SDL_HJALP_H
