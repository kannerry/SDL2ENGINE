#pragma once

#ifndef POWDER_GAMESTATE_H

#define POWDER_GAMESTATE_H

enum PowderType {
	BLANK
};

struct PowderObject {
	int type = BLANK;
	SDL_Color col{ 255, 255, 255 };
};

struct PowderGridPoint {
	Vector2T<int> position;
	PowderObject data;
};

struct PowderGrid {
	int width;
	int height;
	std::vector<std::vector<PowderGridPoint>> grid;
	PowderGrid(int w, int h) : width(w), height(h), grid(w, std::vector<PowderGridPoint>(h)) {}
};

struct PowderGameState {
	PowderGrid pg;
};

#endif // !POWDER_GAMESTATE_H
