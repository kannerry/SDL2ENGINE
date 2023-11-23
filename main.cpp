#pragma once
#define SDL_MAIN_HANDLED
#include "helper_sdl.h"
#include "helper_main.h"

void process_event(SDL_Event *event, WindowContainer *window_container) {
	sdl_default_process_event(event, window_container);
}

void render(WindowContainer* window_container) {
	sdl_default_render_clear(window_container, SDL_Color{ 0, 0, 0, SDL_ALPHA_OPAQUE }); // clear color
	// ^
	Vector2T<int> mouse_position;
	SDL_GetLogicalMouseState(&mouse_position.x, &mouse_position.y, window_container->renderer, window_container->window);
	SDL_SetRenderDrawColor(window_container->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(window_container->renderer, 0, 0, mouse_position.x, mouse_position.y);
	// V
	sdl_default_render_present(window_container); // present
}

int main(int argc, char* args[]) {
	WindowContainer window_container{};
	sdl_init("Window Name", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 200, 100, &window_container, 6);
	SDL_Event event;
	while (window_container.alive) {
		render(&window_container);
		while (SDL_PollEvent(&event)) {
			process_event(&event, &window_container);
		}
	}

	return 0;
}