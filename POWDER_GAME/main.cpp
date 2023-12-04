#pragma once

#define SDL_MAIN_HANDLED
#include "helper_sdl.h"
#include "helper_main.h"
#include "powder_game.h"
#include "misc.h"
#include <string>

void debug_toggle(bool& dbg, SDL_Event* event) {
	if (event->key.keysym.scancode == SDL_SCANCODE_F1 && event->key.state == SDL_RELEASED) {
		dbg = !dbg;
	}
}

void process_event(SDL_Event* event, WindowContainer* window_container) {
	sdl_default_process_event(event, window_container);
}

void render(WindowContainer* window_container, SpriteSheetFont font, bool debug) {
	if (debug) {
		int w; int h;
		SDL_RenderGetLogicalSize(window_container->renderer, &w, &h);
		font.font_color = Vector3T<Uint8>{ 0, 255, 0 };
		Vector2T<int> mouse_position;
		SDL_GetLogicalMouseState(&mouse_position.x, &mouse_position.y, window_container);
		std::string rp = "[" + std::to_string(mouse_position.x) + ", " + std::to_string(mouse_position.y) + "]";
		bool past_L = mouse_position.x >= w / 1.5;
		bool past_B = mouse_position.y >= h / 1.25;
		sdl_draw_text(rp.c_str(), mouse_position + Vector2T<int>(past_L ? -80 : 5, past_B ? -15 : 15), window_container->renderer, font);
		std::string ws = std::to_string(w); std::string hs = std::to_string(h);
		sdl_draw_text((ws+"x"+hs).c_str(), Vector2T<int>(5, 5 + 12), window_container->renderer, font);
	}
	sdl_draw_text(debug ? "(debug)" : "POWDER GAME", Vector2T<int>(5, 5), window_container->renderer, font);
	sdl_default_render_present(window_container);

	sdl_default_render_clear(window_container, SDL_Color{ 16,16,16,SDL_ALPHA_OPAQUE });
}

int main(int argc, char* args[]) {
	WindowContainer window_container{};
	PowderGameState game{};
	bool debug = false;
	sdl_init("POWDER GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 450, &window_container, 2);
	SDL_Event event;
	SpriteSheetFont ssf{};
	while (window_container.alive) {
		ssf.font_color = get_rgb_from_time(SDL_GetTicks64());
		render(&window_container, ssf, debug);
		while (SDL_PollEvent(&event)) {
			process_event(&event, &window_container);
			debug_toggle(debug, &event);
		}
	}
	return 0;
}