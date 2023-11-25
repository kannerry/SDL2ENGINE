#pragma once

#ifndef MISC_PG_H

#define MISC_PG_H
#include "helper_main.h"
#include <SDL2/SDL_stdinc.h>

Vector3T<Uint8> get_rgb_from_time(Uint64 current_time) {
	float frequency = 0.001f;
	Uint8 red = (Uint8)(sin(frequency * current_time + 0) * 127 + 128);
	Uint8 green = (Uint8)(sin(frequency * current_time + 2 * 3.14159265 / 3) * 127 + 128);
	Uint8 blue = (Uint8)(sin(frequency * current_time + 4 * 3.14159265 / 3) * 127 + 128);
	return { red, green, blue };
}

#endif // !MISC_PG_H