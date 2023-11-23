#pragma once
#pragma warning (disable:4244)

#ifndef SDL_HELP_H

#define SDL_HELP_H
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "helper_main.h"

struct WindowContainer {
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    bool alive = true;
};

int sdl_init(const char* title, int xpos, int ypos, int width, int height, WindowContainer* wc, int scale = 1) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return EXIT_FAILURE;
    }
    wc->window = SDL_CreateWindow(
        title,
        xpos,
        ypos,
        width * scale,
        height * scale,
        SDL_WINDOW_SHOWN
    );
    wc->renderer = SDL_CreateRenderer(wc->window, -1, 0);
    SDL_RenderSetLogicalSize(wc->renderer, width, height);
    return 0;
}

void sdl_cleanup(WindowContainer* wc) {
    SDL_DestroyWindow(wc->window);
    SDL_DestroyRenderer(wc->renderer);
    wc->alive = false;
    SDL_Quit();
}

void sdl_get_logical_mouse_position(int* rX, int* rY, SDL_Renderer* r, SDL_Window* w) {
    Vector2T<int> logical_size; Vector2T<int> window_size; Vector2T<int> mouse_position;
    SDL_RenderGetLogicalSize(r, &logical_size.x, &logical_size.y);
    SDL_GetWindowSize(w, &window_size.x, &window_size.y);
    SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
    int x1 = (mouse_position.x * logical_size.x); int x2 = window_size.x;
    int y1 = (mouse_position.y * logical_size.y); int y2 = window_size.y;
    *rX = (mouse_position.x * logical_size.x) / window_size.x;
    *rY = (mouse_position.y * logical_size.y) / window_size.y;
}

int sdl_render_texture(const char* path_to_image, int x, int y, SDL_Renderer* r) {
    SDL_Surface* surface = IMG_Load(path_to_image);
    if (!surface) {
        std::cerr << "!surface:sdl_render_texture @ " << path_to_image << " (" << x << ", " << y << ")\n";
        return EXIT_FAILURE;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(r, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "!texture:sdl_render_texture()\n";
        return EXIT_FAILURE;
    }
    Vector2T<int> texture_size;
    SDL_QueryTexture(texture, NULL, NULL, &texture_size.x, &texture_size.y);
    SDL_Rect texture_rect = { x, y, x + texture_size.x, y + texture_size.y };
    SDL_RenderCopy(r, texture, NULL, &texture_rect);
    SDL_DestroyTexture(texture);
    return 0;
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

#endif // SDL_HELP_H
