# SDL2engine

SDL2engine is currently a barebones engine, with some boilerplate stuff to help you set up a project.

## Installation

The SDL2 & SDL_image libraries are not included, you can use the [VCPKG](https://vcpkg.io/en/) utility to install it using:

```
vcpkg install sdl2
vcpkg install sdl2-image
```

## Stuff
```
WindowContainer { // Contains the SDL window & renderer, as well as an "alive" state. 
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    bool alive = true;
}

int sdl_init(title, xpos, ypos, screen_width, screen_height, window_container, *SCALE*) {
    // There is some logic for a "scaled" renderer where you can have a window
    // that is bigger than your rendered image. example: you don't want a 32x32 window, 
    // but you want a 32x32 sprite to be rendered.

}

void SDL_GetLogicalMouseState(ref_X, refY, window_container) {
    // The scaling logic from above makes it so that the original "SDL_GetMouseState()" return value
    // does not return what you want. Use this to get the point where the mouse is, if you're using
    // screen scaling.
}

int sdl_render_texture(path_to_image, xpos, ypos, renderer) {
    // Takes in a relative image from the exe, displaying it at x, y, using a specific renderer
}

void sdl_default_{process_event}/{render_clear}/{render_present}() {
    // Default setup for drawing a color to the renderer, presenting said renderer, and
    // cleaning up after SDL when quitting.
}
```

## TODO:
```
> More types within 'helper_main.h'
? Default UI (buttons? input boxes?)
? Particle system

```