#pragma once
#define SDL_MAIN_HANDLED
#include "helper_sdl.h"
#include "helper_main.h"

SDL_Color return_debug_rgba_from_launchtime(double freq, int amp) {
	Uint64 launch_time = SDL_GetTicks64();
	int red = static_cast<int>(sin(freq * launch_time + 0) * amp + amp);
	int green = static_cast<int>(sin(freq * launch_time + 2) * amp + amp);
	int blue = static_cast<int>(sin(freq * launch_time + 4) * amp + amp);
	red = std::min(std::max(red, 0), 255);
	green = std::min(std::max(green, 0), 255);
	blue = std::min(std::max(blue, 0), 255);
	return SDL_Color{ static_cast<Uint8>(red), static_cast<Uint8>(green), static_cast<Uint8>(blue), SDL_ALPHA_OPAQUE };
}

void process_event(SDL_Event *event, WindowContainer *window_container) {
	sdl_default_process_event(event, window_container);
}

void render(WindowContainer* window_container) {
	sdl_default_render_clear(window_container, return_debug_rgba_from_launchtime(0.01, 128)); // clear color
	// ^
	
	sdl_render_texture("assets/dev.png", 0, 0, window_container->renderer);

	Vector2T<int> mouse_position; Vector2T<int> render_size;
	sdl_get_logical_mouse_position(&mouse_position.x, &mouse_position.y, window_container->renderer, window_container->window);
	SDL_RenderGetLogicalSize(window_container->renderer, &render_size.x, &render_size.y);
	SDL_SetRenderDrawColor(window_container->renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);

	SDL_RenderDrawLine(window_container->renderer, 0, 0, mouse_position.x, mouse_position.y);
	SDL_RenderDrawLine(window_container->renderer, render_size.x - 1, 0, mouse_position.x, mouse_position.y);
	// V
	sdl_default_render_present(window_container); // present
}

int main(int argc, char* args[]) {
	WindowContainer window_container{};
	sdl_init("kannerENGINETEMP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 100, 100, &window_container, 8);
	SDL_Event event;
	while (window_container.alive) {
		render(&window_container);
		while (SDL_PollEvent(&event)) {
			process_event(&event, &window_container);
		}
	}

	return 0;
}