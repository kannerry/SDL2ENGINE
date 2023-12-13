# SDL2engine

SDL2engine is a small boilerplate repo to help you setup a simple 2D window/game using SDL2

## Installation

The SDL2 & SDL_image libraries are not included, you can try using the [VCPKG](https://vcpkg.io/en/) utility to install it via:

```
vcpkg install sdl2
vcpkg install sdl2-image
```

## General Stuff
```c++
// Contains an SDL window, SDL renderer, SDL event, 
// as well as an "alive" state.
struct WindowContainer { 
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_Event event{};
    bool alive = true;
}

// Contains data for the sdl_render_texture() function
// to render some texture from a path, at x,y.
struct RenderTextureParameters {
	const char* path_to_image; // "assets/foo.png"
	Vector2T<int> position; // "{0, 0}"

	// OPTIONAL PARAMS
	double angle = 0; // rotation
	SDL_RendererFlip flipflags = SDL_FLIP_NONE; // flip H/V
	SDL_Point* pivot = NULL; // rotation point
    Vector3T<Uint8> modulate = { 255, 255, 255 }; // color
    Vector2T<Vector2T<int>> crop_rect{{0, 0}, {0, 0}}; // x, y, w, h
}

// This will attempt to draw to the screen
// some text at a position, using the default
// font; see ENGINE/assets/defaultfont.png
void sdl_draw_text(text, pos, renderer, *optional font){
}

// Referring from the RenderTextureParameters above,
// By default, this function will attempt to draw a texture
// to the screen at x, y; from 0, 0.
void sdl_render_texture(parameters, renderer){
}

// Generally, these are default handlers for the
// SDL pipeline, e.g: process_event will handle 
// cleanup on an event->type == SDL_QUIT.
void sdl_default_something() {
}

```

## TODO:
```
> More types within 'helper_main.h' // objects/entities, timers, etc...
? Default UI // buttons? input boxes? toggles?
```