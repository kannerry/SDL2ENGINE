#pragma once
#pragma warning (disable:4244)

#ifndef SDL_HELP_H

#define SDL_HELP_H
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "helper_main.h"

struct WindowContainer { // contains a window, renderer, and the state of said window/renderer pair
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    bool alive = true;
};

struct RenderTextureParameters {
    const char* path_to_image;
    Vector2T<int> position;
    double angle = 0;
    SDL_RendererFlip flipflags = SDL_FLIP_NONE;
    SDL_Point* pivot = NULL;
    Vector3T<Uint8> modulate = { 255, 255, 255 };
    Vector2T<Vector2T<int>> crop_rect{ {0, 0}, {0, 0} };
};

struct SpriteSheetFont {
    Vector2T<int> character_size{8, 8}; // size of each character in pixels, width & height
    const char* path_to_sheet = "assets/defaultfont.png";
    Vector3T<Uint8> font_color{ 255, 0, 0 };
};

// instantiate all the stuff needed for SDL rendering + some scaling if needed 
// *don't want a 30x30 window, but still want 30x30 pixels to render? try 300x300 (30x30 at 10x scale)*
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

void sdl_cleanup(WindowContainer* wc) { // destroy stuff and deinit sdl
    SDL_DestroyWindow(wc->window);
    SDL_DestroyRenderer(wc->renderer);
    SDL_Quit();
    wc->alive = false;
}

// for scaling purposes: the mouse pos within the window doesn't match up to the renderer when
// SDL_RenderSetLogicalSize() gets called
void SDL_GetLogicalMouseState(int* rX, int* rY, WindowContainer* wc) {
    Vector2T<int> logical_size; Vector2T<int> window_size; Vector2T<int> mouse_position;
    SDL_RenderGetLogicalSize(wc->renderer, &logical_size.x, &logical_size.y);
    SDL_GetWindowSize(wc->window, &window_size.x, &window_size.y);
    SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
    int x1 = (mouse_position.x * logical_size.x); int x2 = window_size.x;
    int y1 = (mouse_position.y * logical_size.y); int y2 = window_size.y;
    *rX = (mouse_position.x * logical_size.x) / window_size.x;
    *rY = (mouse_position.y * logical_size.y) / window_size.y;
}

SDL_Surface* load_image_to_surface(const char* path_to_image) {
    SDL_Surface* surface = IMG_Load(path_to_image);
    if (!surface) {
        std::cerr << "!surface:sdl_render_texture @ " << path_to_image << "\n";
        return NULL;
    }
    return surface;
}

SDL_Texture* create_texture_from_surface(SDL_Renderer* renderer, SDL_Surface* surface) {
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!tex) {
        std::cerr << "!texture:sdl_render_texture()\n";
        return NULL;
    }
    return tex;
}

void __sdl_rtc(RenderTextureParameters param, SDL_Renderer* renderer) {
    SDL_Surface* surface = load_image_to_surface(param.path_to_image);
    SDL_Texture* texture = create_texture_from_surface(renderer, surface);
    // x.xy = xy // y.xy = wh
    SDL_Rect src_rect = { param.crop_rect.x.x, param.crop_rect.x.y, param.crop_rect.y.x, param.crop_rect.y.y };
    SDL_Rect dest_rect = { param.position.x, param.position.y, param.crop_rect.y.x, param.crop_rect.y.y };
    SDL_SetTextureColorMod(texture, param.modulate.x, param.modulate.y, param.modulate.z);
    SDL_RenderCopyEx(renderer, texture, &src_rect, &dest_rect, param.angle, param.pivot, param.flipflags);
    SDL_DestroyTexture(texture);
}

void __sdl_rt(RenderTextureParameters param, SDL_Renderer* renderer) {
    SDL_Surface* surface = load_image_to_surface(param.path_to_image);
    SDL_Texture* texture = create_texture_from_surface(renderer, surface);
    SDL_Rect texture_rect = { param.position.x, param.position.y, 0, 0 };
    SDL_QueryTexture(texture, NULL, NULL, &texture_rect.w, &texture_rect.h);
    SDL_SetTextureColorMod(texture, param.modulate.x, param.modulate.y, param.modulate.z);
    SDL_RenderCopyEx(renderer, texture, NULL, &texture_rect, param.angle, param.pivot, param.flipflags);
    SDL_DestroyTexture(texture);
}

// draw a texture at x, y
void sdl_render_texture(RenderTextureParameters param, SDL_Renderer* renderer) {
    if (param.crop_rect == Vector2T<Vector2T<int>>{ {0, 0}, { 0, 0 } }) // x.y , w.h
    {
        __sdl_rt(param, renderer);
    }
    else {
        __sdl_rtc(param, renderer);
    }
}

void sdl_draw_text(const char* text, Vector2T<int> pos, SDL_Renderer* r, SpriteSheetFont fontsheet = {}) {
    RenderTextureParameters font_param{
        fontsheet.path_to_sheet,
        pos - Vector2T<int>(fontsheet.character_size.x, 0)
    };
    font_param.modulate = fontsheet.font_color;
    int i = 0;
    int pathx = fontsheet.character_size.x;
    int pathy = fontsheet.character_size.y;
    for (; *text != '\0'; ++text) {
        char ch = *text;
        int offset_UC = ch - 'A';
        int offset_LC = ch - 'a';
        int offset_NUM = ch - '0';
        int offset_SYM = ch - '!'; int offset_SYM2 = ch - ':'; int offset_SYM3 = ch - '['; int offset_SYM4 = ch - '{';
        font_param.crop_rect = { {0, 0}, fontsheet.character_size };
        if (offset_UC >= 0 && offset_UC < 25) {
            font_param.position.x += pathx;
            font_param.crop_rect.x.x = (offset_UC * pathx);
        }
        else if (offset_LC >= 0 && offset_LC < 25) {
            font_param.position.x += pathx;
            font_param.crop_rect.x.x = (offset_LC * pathx) + 26 * pathx;
        }
        else if (offset_NUM >= 0 && offset_NUM <= 9){
            font_param.position.x += pathx;
            font_param.crop_rect.x.x = (offset_NUM * pathx);
            font_param.crop_rect.x.y = pathy * 1;
        }
        else if (offset_SYM >= 0 && offset_SYM <= 14) {
            font_param.position.x += pathx;
            font_param.crop_rect.x.x = (offset_SYM * pathx) + 10 * pathx;
            font_param.crop_rect.x.y = pathy * 1;
        }
        else if (offset_SYM2 >= 0 && offset_SYM2 <= 6) {
            font_param.position.x += pathx;
            font_param.crop_rect.x.x = (offset_SYM2 * pathx) + 25 * pathx;
            font_param.crop_rect.x.y = pathy * 1;
        }
        else if (offset_SYM3 >= 0 && offset_SYM3 <= 6) {
            font_param.position.x += pathx;
            font_param.crop_rect.x.x = (offset_SYM3 * pathx) + 32 * pathx;
            font_param.crop_rect.x.y = pathy * 1;
        }
        else if (offset_SYM4 >= 0 && offset_SYM4 <= 4) {
            font_param.position.x += pathx;
            font_param.crop_rect.x.x = (offset_SYM4 * pathx) + 39 * pathx;
            font_param.crop_rect.x.y = pathy * 1;
        }
        else if (ch == ' ') {
            font_param.position.x += 8;
        }
        if (ch != ' ') {
            sdl_render_texture(font_param, r);
        }
        ++i;
    }
}

// defaults
void sdl_default_process_event(SDL_Event* event, WindowContainer* wc) {
    if (event->type == SDL_QUIT) {
        sdl_cleanup(wc);
    }
}

// defaults
void sdl_default_render_clear(WindowContainer* wc, SDL_Color clr) {
    SDL_SetRenderDrawColor(wc->renderer, clr.r, clr.g, clr.b, clr.a);
    SDL_RenderClear(wc->renderer);
}

// defaults
void sdl_default_render_present(WindowContainer* wc) {
    SDL_RenderPresent(wc->renderer);
}

#endif // SDL_HELP_H
