#ifndef PIXEL_ART_H
#define PIXEL_ART_H

#include <SDL2/SDL.h>

void draw_pixel_block(
    SDL_Renderer *renderer,
    int x,
    int y,
    int grid_x,
    int grid_y,
    int width,
    int height,
    int scale
);

void draw_tarnished(SDL_Renderer *renderer, int x, int y);
void draw_misbegotten(SDL_Renderer *renderer, int x, int y);
void draw_demigod(SDL_Renderer *renderer, int x, int y);
void draw_food(SDL_Renderer *renderer, int x, int y);

#endif