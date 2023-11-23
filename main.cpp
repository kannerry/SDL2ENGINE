#pragma once
#define SDL_MAIN_HANDLED
#include "sdl_helper.h"

int main(int argc, char* args[]) {
	WindowContainer window_container{};
	sdl_init("Window Name", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 200, 100, window_container);

	while (window_container.alive) {
		SDL_Event event;
		SDL_PollEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				sdl_cleanup(window_container);
			default:
				break;
		}
	}

	return 0;
}