#pragma once

#include <sdl/SDL.h>

Uint32 get_pixel(SDL_Surface* surface, int x, int y);
void set_pixel(SDL_Surface* surface, int x, int y, Uint32 pixel);