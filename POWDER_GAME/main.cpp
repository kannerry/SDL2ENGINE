#pragma once

#define SDL_MAIN_HANDLED
#include "helper_sdl.h"
#include "helper_main.h"
#include "powder_game.h"
#include "misc.h"
#include <string>

// process stuff within the SDL event, press W? increment an int or something.
void process_event(SDL_Event* event, WindowContainer* window_container) {
	sdl_default_process_event(event, window_container);
}

// render your stuff. have a player position? draw them to x, y within the game
void render(WindowContainer* window_container) {
	sdl_default_render_clear(window_container, SDL_Color{ 16,16,16,SDL_ALPHA_OPAQUE });
	Vector2T<int> mouse_position;
	SDL_GetLogicalMouseState(&mouse_position.x, &mouse_position.y, window_container);
	//
	// TL-name
	SpriteSheetFont ssf{};
	Vector3T<Uint8> col = get_rgb_from_time(SDL_GetTicks64());
	ssf.font_color = col;
	sdl_draw_text("POWDER GAME", Vector2T<int>(5, 5), window_container->renderer, ssf);
	//

	RenderTextureParameters cursor{
	"assets/cursor.png",
	mouse_position
	};
	cursor.modulate = col;
	sdl_render_texture(cursor, window_container->renderer);
	std::string rp = "(" + std::to_string(mouse_position.x) + ", " + std::to_string(mouse_position.y) + ")";
	sdl_draw_text(rp.c_str(), mouse_position + Vector2T<int>(15, 15), window_container->renderer, ssf);

	sdl_default_render_present(window_container); // present
}

int main(int argc, char* args[]) { // main func. gets called when opening the exe
	WindowContainer window_container{};
	PowderGameState game{};
	sdl_init("POWDER GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 450, &window_container, 2);
	SDL_Event event;
	while (window_container.alive) {
		render(&window_container);
		while (SDL_PollEvent(&event)) {
			process_event(&event, &window_container);
		}
	}

	return 0;
}