#include <sdl/SDL.h>
#include <stdio.h>
#include <string>

#include "quadtree.h"
#include "utils.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int BLAST_RADIUS = 128;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface* screen_surf = NULL;
SDL_Surface* sky_surf = NULL;
SDL_Surface* ground_surf = NULL;
SDL_Surface* blast_surf = NULL;

void exit_with_err(const std::string& msg) {
    printf("%s: %s\n", msg.c_str(), SDL_GetError());
    exit(1);
}

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        exit_with_err("SDL_Init error");
        return false;
    }

    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        exit_with_err("SDL_CreateWindow error");
        return false;
    }

    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    if( renderer == NULL )
    {
        SDL_Log("Accelerated renderer could not be created");
        renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_SOFTWARE);
    }
    if( renderer == NULL )
    {
        exit_with_err("Renderer could not be created");
    }

    screen_surf = SDL_GetWindowSurface(window);
    return 0;
}

void close() {
    if (ground_surf != NULL) {
        SDL_FreeSurface(ground_surf);
        ground_surf = NULL;
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();
}


SDL_Surface* load_bmp_surf(char const* path) {
    SDL_Surface* surf = SDL_LoadBMP(path);
    if (!surf) {
        exit_with_err("Could not load bmp");
    }
    return surf;

}

int main(int argc, char* args[])
{
    init();

    ground_surf = load_bmp_surf("ground.bmp");
    blast_surf = load_bmp_surf("blast.bmp");
    sky_surf = load_bmp_surf("sky.bmp");

    Node *root = new Node;

    // insert_pixels(
    //     root,
    //     ground_surf,
    //     SDL_Rect { 0, 0, ground_surf->w, ground_surf->h }
    //     // SDL_Rect { 0, 0, 10, 10 }
    // );

    // Set top left most pixel color as transparency color
    Uint32 ground_alpha_pix = get_pixel(ground_surf, 0, 0);
    Uint32 blast_alpha_pix = get_pixel(blast_surf, 0, 0);
    Uint32 blast_impact_pix = get_pixel(blast_surf, BLAST_RADIUS / 2, BLAST_RADIUS / 2);

    SDL_Rect blast_rect{ 0, 0, BLAST_RADIUS, BLAST_RADIUS };

    SDL_BlitSurface(
        ground_surf,
        NULL,
        screen_surf,
        NULL
    );
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        SDL_BlitSurface(
            sky_surf,
            NULL,
            screen_surf,
            NULL
        );

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_MOUSEMOTION) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                blast_rect.x = x - BLAST_RADIUS / 2;
                blast_rect.y = y - BLAST_RADIUS / 2;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                blast_rect.x = x - BLAST_RADIUS / 2;
                blast_rect.y = y - BLAST_RADIUS / 2;

                // iterate through ground pixels

                // blast
                Uint8 br, bg, bb, ba;
                // ground
                Uint8 gr, gg, gb, ga;

                Uint32* ground_pixels = (Uint32*)ground_surf->pixels;
                Uint32* blast_pixels = (Uint32*)blast_surf->pixels;
                int ground_transparent = 0;

                for (int i = 0; i < BLAST_RADIUS * BLAST_RADIUS; i++) {
                    int blast_y = i / BLAST_RADIUS;
                    int blast_x = i % BLAST_RADIUS;

                    int ground_y = blast_rect.y + blast_y;
                    int ground_x = blast_rect.x + blast_x;

                    Uint32 blast_pix = get_pixel(blast_surf, blast_x, blast_y);
                    Uint32 ground_pix = get_pixel(ground_surf, ground_x, ground_y);

                    if (ground_pix != ground_alpha_pix && blast_pix != blast_alpha_pix) {
                        if (blast_pix == blast_impact_pix) {
                            set_pixel(ground_surf, ground_x, ground_y, ground_alpha_pix);
                        }
                        else {
                            set_pixel(ground_surf, ground_x, ground_y, blast_pix);
                        }
                    }
                }
            }
        }
        SDL_BlitSurface(
            ground_surf,
            NULL,
            screen_surf,
            NULL
        );
        SDL_BlitSurface(
            blast_surf,
            NULL,
            screen_surf,
            &blast_rect
        );

        SDL_UpdateWindowSurface(window);

        // SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
        // SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        // SDL_RenderFillRect(renderer, &fillRect);

        // SDL_RenderPresent( renderer );
    }

    close();
    return 0;
}
