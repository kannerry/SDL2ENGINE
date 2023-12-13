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

void set_point_to_type(PowderGameState* game, Vector2T<int> point, PowderType type) {
	if (point.x < 0 || point.y < 0 || point.x > game->width || point.y > game->height) {
		return;
	}
	switch (type)
	{
		case WATER:
			game->data[point.x][point.y].col = SDL_Color{ 0, 0, 255, 255 };
			break;
		case SOLID:
			game->data[point.x][point.y].col = SDL_Color{ 128, 100, 80 };
			break;
	}
	game->data[point.x][point.y].type = type;
}

void process_event(SDL_Event* event, WindowContainer* window_container, PowderGameState* game) {
	sdl_default_process_event(event, window_container);
	Vector2T<int> mouse_position;
	Vector2T<int> ls;
	SDL_GetLogicalMouseState(&mouse_position.x, &mouse_position.y, window_container);
	SDL_RenderGetLogicalSize(window_container->renderer, &ls.x, &ls.y);
	if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_LMASK) {
		for (const auto& point : get_points_from_radius(mouse_position, 3)) {
			if (point.x == ls.x || point.y == ls.y) { return; };
			set_point_to_type(game, point, game->selected_type);
		}
	}
	if (event->type == SDL_MOUSEWHEEL) {
		if (event->wheel.y > 0) {
			game->selected_type = static_cast<PowderType>((game->selected_type + 1) % (MAXTYPE));
		}
		else if (event->wheel.y < 0) {
			auto next = static_cast<PowderType>((game->selected_type - 1) % (MAXTYPE));
			if (next == -1) {
				next = static_cast<PowderType>(MAXTYPE - 1);
			}
			game->selected_type = next;
		}
	}
}

void render_game(WindowContainer* window_container, bool debug, SpriteSheetFont font, PowderGameState* game) {
	Vector2T<int> mouse_position;
	SDL_GetLogicalMouseState(&mouse_position.x, &mouse_position.y, window_container);
	if (debug)
	{
		//sdl_draw_text_to_mouse(type_string(static_cast<PowderType>(under_type)).c_str(), Vector2T<int>(0, 0), window_container, font);
	}
	else {
		sdl_draw_text_to_mouse(type_string(game->selected_type).c_str(), Vector2T<int>(8, 8), window_container, font);
	}
	for (size_t ih = 0; ih < game->height; ih++)
	{
		for (size_t iw = 0; iw < game->width; iw++)
		{
			if (game->data[iw][ih].type != BLANK) {
				PowderObject p = game->data[iw][ih];
				SDL_SetRenderDrawColor(window_container->renderer, p.col.r, p.col.g, p.col.b, 255);
				SDL_RenderDrawPoint(window_container->renderer, iw, ih);
			}
		}
	}
}

bool random_bool() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 1);
	return dis(gen) == 1;
}

void process_game(PowderGameState* game) {
	for (size_t iw = 0; iw < game->width; iw++)
	{
		for (size_t ih = game->height - 1; ih >= 1; ih--)
		{
			if (game->data[iw][ih].type == WATER) {
				bool down = false;
				bool left = false; bool right = false;
				if (ih + 1 < game->height && game->data[iw][ih + 1].type == BLANK) { // i can go down
					down = true;
				}
				if (iw + 1 < game->width && game->data[iw + 1][ih].type == BLANK) {
					right = true;
				}
				if (iw - 1 > 0 && game->data[iw - 1][ih].type == BLANK) {
					left = true;
				}
				if (down) {
					game->data[iw][ih].type = BLANK;
					set_point_to_type(game, Vector2T<int>(iw, ih + 1), WATER);
				}
				else {
					if (left && right) {
						bool lor = random_bool();
						int dir = lor ? -1 : 1;
						game->data[iw][ih].type = BLANK;
						set_point_to_type(game, Vector2T<int>(iw - dir, ih), WATER);
						break;
					}
					else {
						if (right) {
							game->data[iw][ih].type = BLANK;
							set_point_to_type(game, Vector2T<int>(iw + 1, ih), WATER);
						}
						if (left) {
							game->data[iw][ih].type = BLANK;
							set_point_to_type(game, Vector2T<int>(iw - 1, ih), WATER);
						}
					}
				}
			}
		}
	}
}

void render_debug(WindowContainer* window_container, bool debug, SpriteSheetFont font) {
	Vector2T<int> mouse_position;
	SDL_GetLogicalMouseState(&mouse_position.x, &mouse_position.y, window_container);
	int w; int h;
	SDL_RenderGetLogicalSize(window_container->renderer, &w, &h);
	bool past_L = mouse_position.x >= w / 1.5;
	bool past_B = mouse_position.y >= h / 1.25;
	if (debug) {
		font.font_color = Vector3T<Uint8>{ 0, 255, 0 };
		std::string rp = "[" + std::to_string(mouse_position.x) + ", " + std::to_string(mouse_position.y) + "]";
		sdl_draw_text(rp.c_str(), Vector2T<int>(5, 28), window_container->renderer, font);
		std::string ws = std::to_string(w); std::string hs = std::to_string(h);
		sdl_draw_text((ws + "x" + hs).c_str(), Vector2T<int>(5, 16), window_container->renderer, font);
	}
	sdl_draw_text(debug ? "(debug)" : "POWDER GAME", Vector2T<int>(5, 5), window_container->renderer, font);
}

void render(WindowContainer* window_container, PowderGameState* game, bool debug, SpriteSheetFont font) {
	sdl_default_render_clear(window_container, SDL_Color{ 16,16,16,SDL_ALPHA_OPAQUE });
	render_game(window_container, debug, font, game);
	render_debug(window_container, debug, font);
	sdl_default_render_present(window_container);
}

int main(int argc, char* args[]) {
	WindowContainer window_container{};
	bool debug = false;
	int w = 800; int h = 450;
	sdl_init("POWDER GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, &window_container, 2);
	PowderGameState game{ w, h };
	SDL_Event event;
	SpriteSheetFont ssf{};
	while (window_container.alive) {
		int frame_start = SDL_GetTicks64();
		ssf.font_color = get_rgb_from_time(SDL_GetTicks64());
		render(&window_container, &game, debug, ssf);
		while (SDL_PollEvent(&event)) {
			process_event(&event, &window_container, &game);
			debug_toggle(debug, &event);
		}
		process_game(&game);
		int frame_end = SDL_GetTicks64() - frame_start;
		if (frame_end < 1000/60) {
			SDL_Delay(1000/60 - frame_end);
		}
	}
	return 0;
}