#pragma once

#include <sdl/SDL.h>
#include <stdio.h>

#include "utils.h"

struct Node {
    Node *children[4];
    int count;

    SDL_Rect rect;
    Uint32 pixel;
};


bool need_split(SDL_Surface *surf, SDL_Rect r);
void insert_pixels(Node *n, SDL_Surface *surf, SDL_Rect r);
