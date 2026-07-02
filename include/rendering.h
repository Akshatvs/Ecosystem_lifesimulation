#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "types.h"

void draw_individual(SDL_Renderer *renderer, Individual *individual);

void draw_simulation_entities(
    SDL_Renderer *renderer,
    Simulation *simulation
);

void draw_selected_individual_stats(
    SDL_Renderer *renderer,
    Individual *individual,
    TTF_Font *font
);

void draw_inventory_panel(
    SDL_Renderer *renderer,
    Simulation *simulation,
    TTF_Font *font,
    SDL_Texture *inventory_background
);

#endif