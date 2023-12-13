#pragma once

#define SDL_MAIN_HANDLED
#include "helper_sdl.h"
#include "helper_main.h"

// process stuff
void process_event(SDL_Event *event, WindowContainer *window_container) {
	sdl_default_process_event(event, window_container);
}

// render stuff
void render(WindowContainer* window_container) {
	sdl_default_render_clear(window_container, SDL_Color{ 0,0,0,SDL_ALPHA_OPAQUE }); // make background RGBA(0,0,0,255)
	// get mouse position
	Vector2T<int> mouse_position;
	SDL_GetLogicalMousePosition(&mouse_position.x, &mouse_position.y, window_container);
	// draw a point of color RGBA(255,255,255,255);
	SDL_SetRenderDrawColor(window_container->renderer, 255, 255, 255, 255);
	// at the mouse pointer
	SDL_RenderDrawPoint(window_container->renderer, mouse_position.x, mouse_position.y);
	//

	sdl_default_render_present(window_container); // present
}

WindowContainer window_container{};
int main(int argc, char* args[]) { // main func. gets called when opening the exe
	sdl_init("kannerENGINE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 100, 100, &window_container, 8); // init sdl
	while (window_container.alive) { // while window wants to be alive
		render(&window_container); // render it
		while (SDL_PollEvent(&window_container.event)) { // poll and then
			process_event(&window_container.event, &window_container); // process events
		}
	}

	return 0;
}