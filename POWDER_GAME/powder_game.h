#pragma once

#ifndef POWDER_GAMESTATE_H

#define POWDER_GAMESTATE_H

enum PowderType {
	BLANK,
	SOLID,
	WATER,
	MAXTYPE
};

std::string type_string(PowderType type) {
	switch (type) {
		case BLANK: return "Blank";
		case SOLID: return "Ground";
		case WATER: return "Water";
		case MAXTYPE: return "";
		default: return "UNKNOWN";
	}
}

struct PowderObject {
	int type = BLANK;
	SDL_Color col{ 255, 255, 255 };
};

struct PowderGameState {
	int width;
	int height;
	std::vector<std::vector<PowderObject>> data;
	PowderType selected_type = SOLID;
	PowderGameState(int w, int h) : width(w), height(h), data(std::vector<std::vector<PowderObject>>(w, std::vector<PowderObject>(h))) {}
};

#endif // !POWDER_GAMESTATE_H
