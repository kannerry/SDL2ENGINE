#pragma once

#ifndef GAMESTATE_H

#define GAMESTATE_H
#include "helper_main.h"
#include <deque>

enum PlayerFacingDirection {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

struct SnakeGameState
{
	Vector2T<int> player_head; // position of the player head
	Vector2T<int> fruit_pos; // position of le fruit
	std::deque<Vector2T<int>> player_body; // double ended queue for the player body
	int player_length;
	PlayerFacingDirection facing = EAST; // facing direction of the player
	SDL_Event buffered_move{};
	int desired_fps;
	int apples_eated;
};

bool chunk_is_at(Vector2T<int> position, SnakeGameState* gs) {
	for (Vector2T<int>& segment : gs->player_body)
	{
		if (segment.x == position.x && segment.y == position.y) {
			return true;
		}
	}
	return false;
}

Vector2T<int> direction_to_vector(PlayerFacingDirection dir) {
	switch (dir) {
		case NORTH:
			return Vector2T<int>(0, -1);
		case SOUTH:
			return Vector2T<int>(0, 1);
		case EAST:
			return Vector2T<int>(1, 0);
		case WEST:
			return Vector2T<int>(-1, 0);
		default:
			return Vector2T<int>(0, 0);
	}
}

bool is_within_bounds(Vector2T<int> pos, int minX, int maxX, int minY, int maxY) {
	return (pos.x >= minX && pos.x <= maxX && pos.y >= minY && pos.y <= maxY);
}

int rng_randintrange(int lower_bound, int upper_bound) {
	std::random_device random_device;
	std::mt19937 generator(random_device());
	std::uniform_int_distribution<int> distribution(lower_bound, upper_bound);
	int random_number = distribution(generator);
	return random_number;
}

#endif // !GAMESTATE_H
