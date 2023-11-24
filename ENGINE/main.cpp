#pragma once

#define SDL_MAIN_HANDLED
#include "helper_sdl.h"
#include "helper_main.h"

// process stuff within the SDL event, press W? increment an int or something.
void process_event(SDL_Event *event, WindowContainer *window_container) {
	sdl_default_process_event(event, window_container);
}

// render your stuff. have a player position? draw them to x, y within the game
void render(WindowContainer* window_container) {
	sdl_default_render_clear(window_container, SDL_Color{ 0,0,0,SDL_ALPHA_OPAQUE });
	//

	// std::cout << "Place STUFF here, to render.\n";
	Vector2T<int> mouse_position; Vector2T<int> renderer_size;
	SDL_GetLogicalMouseState(&mouse_position.x, &mouse_position.y, window_container);
	SDL_RenderGetLogicalSize(window_container->renderer, &renderer_size.x, &renderer_size.y);

	SDL_SetRenderDrawColor(window_container->renderer, 255, 255, 255, 255);

	SDL_RenderDrawLine(window_container->renderer, 0, 0, mouse_position.x, mouse_position.y);
	SDL_RenderDrawLine(window_container->renderer, renderer_size.x, renderer_size.y, mouse_position.x, mouse_position.y);
	//

	sdl_default_render_present(window_container); // present
}

int main(int argc, char* args[]) { // main func. gets called when opening the exe
	WindowContainer window_container{};
	sdl_init("kannerENGINE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 100, 100, &window_container, 8);
	SDL_Event event;
	while (window_container.alive) {
		render(&window_container);
		while (SDL_PollEvent(&event)) {
			process_event(&event, &window_container);
		}
	}

	return 0;
}