#pragma once
#define SDL_MAIN_HANDLED

#include "helper_sdl.h" // because of includes, these technically the exact same as the base "ENGINE" helpers,
#include "helper_main.h" // from the base project, but you can do this however you want in your game ^_^

#include "game_state.h" // this one is not from "ENGINE" so it's cool ¬_¬
#include <string>;

// THOUGHTS:
// instead of moving the player on an 8x8 grid instantly, it would look nicer to
// transition him between each tile smoothly. kind of like how pokemon does it!
// another feature would be to display the "score" / fruits eaten directly on
// the screen, that's something that would be interesting to implement. feel free
// to try and do it!

const int default_player_length = 5;
const int default_base_fps_cap = 12;

void render_snake_chunk(int x, int y, SDL_Renderer* r) {
	sdl_render_texture(
		"assets/snake_chunk.png",
		x,
		y,
		r, 
		0,
		NULL,
		SDL_FLIP_NONE
	);
}

void render(WindowContainer* window_container, SnakeGameState *gs) {
	sdl_default_render_clear(window_container, SDL_Color{ 0,0,0,SDL_ALPHA_OPAQUE });

	// the assets will be pushed to the build directory by adding
	// `xcopy "$(ProjectDir)assets\*.*" "$(TargetDir)assets" /Y /I /E`
	// to the build events within our project. //*sdl_render_texture() eats them relative to the EXE*//

	for (Vector2T<int> &segment : gs->player_body)
	{
		render_snake_chunk(segment.x * 8, segment.y * 8, window_container->renderer);
	}

	// todo: multiple fruits
	// fruit
	sdl_render_texture("assets/apple.png", gs->fruit_pos.x * 8, gs->fruit_pos.y * 8, window_container->renderer);

	// render head
	render_snake_chunk(gs->player_head.x * 8, gs->player_head.y * 8, window_container->renderer);

	// facing indicator
	int rot{};
	SDL_RendererFlip flipdir{};
	switch (gs->facing)
	{
		case NORTH:
			rot = 0;
			flipdir = SDL_FLIP_NONE;
			break;
		case SOUTH:
			rot = 0;
			flipdir = SDL_FLIP_VERTICAL;
			break;
		case EAST:
			rot = 90;
			flipdir = SDL_FLIP_NONE;
			break;
		case WEST:
			rot = -90;
			flipdir = SDL_FLIP_NONE;
		default:
			break;
	}
	// facing indicator for the char
	sdl_render_texture(
		"assets/facing_indicator.png",
		gs->player_head.x * 8,
		gs->player_head.y * 8,
		window_container->renderer,
		rot,
		NULL,
		flipdir
	);
	//

	sdl_default_render_present(window_container);
}

void process_move(SDL_Event* e, SnakeGameState* gs, bool &input_processed) {
	if (e->type == SDL_KEYDOWN) {
		// i wonder if there's an easier way to do this ... :^(
		switch (e->key.keysym.sym) {
			case SDLK_UP:
			case SDLK_w:
				if (gs->facing != SOUTH) {
					input_processed = true;
					gs->facing = NORTH;
				}
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				if (gs->facing != WEST) {
					input_processed = true;
					gs->facing = EAST;
				}
				break;
			case SDLK_DOWN:
			case SDLK_s:
				if (gs->facing != NORTH) {
					input_processed = true;
					gs->facing = SOUTH;
				}
				break;
			case SDLK_LEFT:
			case SDLK_a:
				if (gs->facing != EAST) {
					input_processed = true;
					gs->facing = WEST;
				}
				break;
			default:
				break;
		}
	}
}

void process_event(SDL_Event *event, WindowContainer *window_container, SnakeGameState *gs) {
	sdl_default_process_event(event, window_container);
}

void process_game(SnakeGameState *gs, WindowContainer *wc) {
	std::string title = "SNAKE -- apples eaten: " + std::to_string(gs->apples_eated);
	SDL_SetWindowTitle(wc->window, title.c_str());
	gs->player_body.push_front(gs->player_head);
	if (gs->player_head == gs->fruit_pos) {
		gs->apples_eated += 1;
		gs->player_length += 1;
		gs->desired_fps += 1;
		Vector2T<int> fruit_pos{ rng_randintrange(0, 47), rng_randintrange(0, 25) };
		while (chunk_is_at(fruit_pos, gs)) {
			std::cout << "HIT";
			fruit_pos.x = rng_randintrange(0, 47);
			fruit_pos.y = rng_randintrange(0, 25);
		}
		gs->fruit_pos = fruit_pos;
	}
	Vector2T<int> position_infront = gs->player_head + direction_to_vector(gs->facing);
	while (gs->player_body.size() > gs->player_length)
	{
		gs->player_body.pop_back();
	}
	if (chunk_is_at(position_infront, gs) || !is_within_bounds(position_infront, 0, 47, 0, 26))
	{
		int precheck = gs->player_length - 3;
		if (precheck < 0) {
			gs->player_length = 0;
		}
		else {
			gs->player_length -= 1;
			gs->player_body.pop_back();
		}
		return;
	}
	switch (gs->facing)
	{
		case NORTH:
			gs->player_head.y -= 1;
			break;
		case SOUTH:
			gs->player_head.y += 1;
			break;
		case EAST:
			gs->player_head.x += 1;
			break;
		case WEST:
			gs->player_head.x -= 1;
			break;
		default:
			break;
	}

	if (gs->buffered_move.key.keysym.sym != SDLK_F24) {
		bool temp = true;
		process_move(&gs->buffered_move, gs, temp);
		gs->buffered_move.key.keysym.sym = SDLK_F24;
	}
}

int main(int argc, char* args[]) {
	WindowContainer window_container{};
	// 16:9
	// 16 * 8 = 128
	// 9 * 8 = 72
	// the "game world" is technically 48w26h, but we display on an 8x8 px grid (arbitrary)
	Vector2T<int> render_resolution { 384, 216 };
	SnakeGameState game{};
	// want the starting pos of the player to be in the middle of the world, on the left
	// the world is in blocks of 8, so we divide by 8. then we want to be in the middle, so half the total res on y and 1x is the middle left we want
	game.player_head = Vector2T<int>(1, (render_resolution.y / 8) / 2);
	game.player_length = default_player_length;
	game.desired_fps = default_base_fps_cap;
	// and we want a fruit to start us off (nearby)
	game.fruit_pos.x = game.player_head.x + rng_randintrange(1, 3);
	game.fruit_pos.y = game.player_head.y + rng_randintrange(1, 3);
	// init
	bool input_processed = false;
	Uint32 frame_start;
	int frame_time;
	SDL_Event event;
	sdl_init("SNAKE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, render_resolution.x, render_resolution.y, &window_container, 4);
	while (window_container.alive) {
		// reset the buffered move to f24 (arbitrary buffer to ignore)
		// dynamic fps: make the game run faster, the more apples you eat
		const int frame_delay = 1000 / game.desired_fps;
		// get the time before the frame is handled
		frame_start = SDL_GetTicks64();
		while (SDL_PollEvent(&event)) {
			// default process
			process_event(&event, &window_container, &game);
			// if we want to exit, skip rendering (window & renderer has been cleaned already) helper_sdl.h@LINE40
			if (window_container.alive == false) {
				goto exit_cleanup;
			}
			// only process the first move
			if (!input_processed) {
				process_move(&event, &game, input_processed);
			}
			// otherwise buffer it
			else {
				game.buffered_move = event;
			}
		}
		render(&window_container, &game);
		process_game(&game, &window_container);
		// handle the frame
		input_processed = false;
		// calculate the time it took to handle the frame
		frame_time = SDL_GetTicks64() - frame_start;
		// if its not the fps we want
		if (frame_delay > frame_time) {
			// wait until it is
			SDL_Delay(frame_delay - frame_time);
		}
	}
	exit_cleanup:
	return 0;
}