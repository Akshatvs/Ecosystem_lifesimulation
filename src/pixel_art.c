#include "pixel_art.h"

void draw_pixel_block(
    SDL_Renderer *renderer,
    int x,
    int y,
    int grid_x,
    int grid_y,
    int width,
    int height,
    int scale
)
{
    SDL_Rect block;

    block.x = x + grid_x * scale;
    block.y = y + grid_y * scale;
    block.w = width * scale;
    block.h = height * scale;

    SDL_RenderFillRect(renderer, &block);
}

void draw_tarnished(SDL_Renderer *renderer, int x, int y)
{
    int scale = 2;

    SDL_SetRenderDrawColor(renderer, 18, 18, 18, 255);
    draw_pixel_block(renderer, x, y, 5, 0, 6, 1, scale);
    draw_pixel_block(renderer, x, y, 4, 1, 8, 1, scale);
    draw_pixel_block(renderer, x, y, 3, 2, 10, 2, scale);
    draw_pixel_block(renderer, x, y, 2, 4, 12, 7, scale);
    draw_pixel_block(renderer, x, y, 1, 11, 5, 4, scale);
    draw_pixel_block(renderer, x, y, 10, 11, 5, 4, scale);
    draw_pixel_block(renderer, x, y, 0, 15, 6, 2, scale);
    draw_pixel_block(renderer, x, y, 10, 15, 6, 2, scale);

    SDL_SetRenderDrawColor(renderer, 105, 105, 105, 255);
    draw_pixel_block(renderer, x, y, 5, 2, 6, 2, scale);
    draw_pixel_block(renderer, x, y, 4, 4, 8, 2, scale);
    draw_pixel_block(renderer, x, y, 5, 6, 6, 4, scale);
    draw_pixel_block(renderer, x, y, 6, 10, 4, 4, scale);
    draw_pixel_block(renderer, x, y, 2, 12, 3, 2, scale);
    draw_pixel_block(renderer, x, y, 11, 12, 3, 2, scale);

    SDL_SetRenderDrawColor(renderer, 170, 170, 170, 255);
    draw_pixel_block(renderer, x, y, 6, 3, 4, 1, scale);
    draw_pixel_block(renderer, x, y, 7, 6, 2, 3, scale);
    draw_pixel_block(renderer, x, y, 6, 12, 4, 1, scale);

    SDL_SetRenderDrawColor(renderer, 60, 48, 35, 255);
    draw_pixel_block(renderer, x, y, 5, 5, 6, 2, scale);
    draw_pixel_block(renderer, x, y, 5, 11, 2, 4, scale);
    draw_pixel_block(renderer, x, y, 9, 11, 2, 4, scale);

    SDL_SetRenderDrawColor(renderer, 90, 0, 0, 255);
    draw_pixel_block(renderer, x, y, 4, 15, 3, 1, scale);
    draw_pixel_block(renderer, x, y, 9, 15, 3, 1, scale);
}

void draw_misbegotten(SDL_Renderer *renderer, int x, int y)
{
    int scale = 2;

    SDL_SetRenderDrawColor(renderer, 22, 22, 22, 255);
    draw_pixel_block(renderer, x, y, 5, 1, 6, 1, scale);
    draw_pixel_block(renderer, x, y, 4, 2, 8, 2, scale);
    draw_pixel_block(renderer, x, y, 3, 4, 10, 3, scale);
    draw_pixel_block(renderer, x, y, 2, 7, 12, 4, scale);
    draw_pixel_block(renderer, x, y, 1, 11, 5, 4, scale);
    draw_pixel_block(renderer, x, y, 10, 11, 5, 4, scale);

    SDL_SetRenderDrawColor(renderer, 120, 112, 105, 255);
    draw_pixel_block(renderer, x, y, 5, 2, 6, 2, scale);
    draw_pixel_block(renderer, x, y, 4, 4, 8, 3, scale);
    draw_pixel_block(renderer, x, y, 5, 7, 6, 4, scale);

    SDL_SetRenderDrawColor(renderer, 210, 210, 195, 255);
    draw_pixel_block(renderer, x, y, 5, 0, 6, 1, scale);
    draw_pixel_block(renderer, x, y, 4, 1, 1, 2, scale);
    draw_pixel_block(renderer, x, y, 11, 1, 1, 2, scale);

    SDL_SetRenderDrawColor(renderer, 70, 40, 35, 255);
    draw_pixel_block(renderer, x, y, 5, 5, 6, 2, scale);
    draw_pixel_block(renderer, x, y, 4, 10, 3, 3, scale);
    draw_pixel_block(renderer, x, y, 9, 10, 3, 3, scale);

    SDL_SetRenderDrawColor(renderer, 160, 50, 40, 255);
    draw_pixel_block(renderer, x, y, 6, 4, 1, 1, scale);
    draw_pixel_block(renderer, x, y, 9, 4, 1, 1, scale);

    SDL_SetRenderDrawColor(renderer, 95, 75, 55, 255);
    draw_pixel_block(renderer, x, y, 1, 7, 4, 1, scale);
    draw_pixel_block(renderer, x, y, 11, 7, 4, 1, scale);
    draw_pixel_block(renderer, x, y, 0, 6, 2, 1, scale);
    draw_pixel_block(renderer, x, y, 14, 6, 2, 1, scale);

    SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
    draw_pixel_block(renderer, x, y, 0, 5, 3, 1, scale);
    draw_pixel_block(renderer, x, y, 13, 5, 3, 1, scale);
}

void draw_demigod(SDL_Renderer *renderer, int x, int y)
{
    int scale = 2;

    SDL_SetRenderDrawColor(renderer, 80, 20, 15, 255);
    draw_pixel_block(renderer, x, y, 5, 0, 6, 2, scale);
    draw_pixel_block(renderer, x, y, 4, 2, 8, 2, scale);
    draw_pixel_block(renderer, x, y, 3, 4, 10, 1, scale);

    SDL_SetRenderDrawColor(renderer, 230, 190, 60, 255);
    draw_pixel_block(renderer, x, y, 5, 4, 6, 2, scale);
    draw_pixel_block(renderer, x, y, 4, 6, 8, 5, scale);
    draw_pixel_block(renderer, x, y, 3, 11, 10, 2, scale);
    draw_pixel_block(renderer, x, y, 5, 13, 2, 3, scale);
    draw_pixel_block(renderer, x, y, 9, 13, 2, 3, scale);

    SDL_SetRenderDrawColor(renderer, 255, 220, 90, 255);
    draw_pixel_block(renderer, x, y, 6, 5, 4, 1, scale);
    draw_pixel_block(renderer, x, y, 6, 7, 4, 2, scale);
    draw_pixel_block(renderer, x, y, 5, 11, 6, 1, scale);

    SDL_SetRenderDrawColor(renderer, 110, 30, 25, 255);
    draw_pixel_block(renderer, x, y, 2, 9, 3, 5, scale);
    draw_pixel_block(renderer, x, y, 11, 9, 3, 5, scale);

    SDL_SetRenderDrawColor(renderer, 70, 55, 55, 255);
    draw_pixel_block(renderer, x, y, 1, 6, 3, 2, scale);
    draw_pixel_block(renderer, x, y, 12, 6, 3, 2, scale);
    draw_pixel_block(renderer, x, y, 0, 5, 2, 1, scale);
    draw_pixel_block(renderer, x, y, 14, 5, 2, 1, scale);

    SDL_SetRenderDrawColor(renderer, 120, 80, 160, 255);
    draw_pixel_block(renderer, x, y, 0, 4, 2, 1, scale);
    draw_pixel_block(renderer, x, y, 14, 4, 2, 1, scale);

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    draw_pixel_block(renderer, x, y, 6, 16, 4, 1, scale);
}

void draw_food(SDL_Renderer *renderer, int x, int y)
{
    int scale = 2;

    SDL_SetRenderDrawColor(renderer, 15, 10, 10, 255);
    draw_pixel_block(renderer, x, y, 6, 0, 3, 1, scale);
    draw_pixel_block(renderer, x, y, 5, 1, 4, 1, scale);
    draw_pixel_block(renderer, x, y, 3, 2, 6, 2, scale);
    draw_pixel_block(renderer, x, y, 1, 4, 7, 3, scale);
    draw_pixel_block(renderer, x, y, 0, 7, 6, 2, scale);
    draw_pixel_block(renderer, x, y, 1, 9, 4, 1, scale);

    SDL_SetRenderDrawColor(renderer, 210, 190, 150, 255);
    draw_pixel_block(renderer, x, y, 7, 0, 2, 1, scale);
    draw_pixel_block(renderer, x, y, 6, 1, 3, 1, scale);
    draw_pixel_block(renderer, x, y, 5, 2, 2, 1, scale);

    SDL_SetRenderDrawColor(renderer, 160, 140, 100, 255);
    draw_pixel_block(renderer, x, y, 4, 2, 2, 1, scale);
    draw_pixel_block(renderer, x, y, 4, 3, 2, 1, scale);

    SDL_SetRenderDrawColor(renderer, 55, 22, 18, 255);
    draw_pixel_block(renderer, x, y, 3, 3, 3, 2, scale);
    draw_pixel_block(renderer, x, y, 2, 5, 4, 2, scale);
    draw_pixel_block(renderer, x, y, 1, 7, 3, 1, scale);

    SDL_SetRenderDrawColor(renderer, 110, 45, 35, 255);
    draw_pixel_block(renderer, x, y, 4, 4, 3, 2, scale);
    draw_pixel_block(renderer, x, y, 3, 6, 3, 2, scale);
    draw_pixel_block(renderer, x, y, 2, 8, 2, 1, scale);

    SDL_SetRenderDrawColor(renderer, 160, 65, 45, 255);
    draw_pixel_block(renderer, x, y, 5, 5, 2, 1, scale);
    draw_pixel_block(renderer, x, y, 4, 7, 2, 1, scale);

    SDL_SetRenderDrawColor(renderer, 200, 100, 20, 255);
    draw_pixel_block(renderer, x, y, 1, 6, 2, 2, scale);
    draw_pixel_block(renderer, x, y, 0, 8, 3, 1, scale);
    draw_pixel_block(renderer, x, y, 1, 9, 2, 1, scale);

    SDL_SetRenderDrawColor(renderer, 240, 160, 30, 255);
    draw_pixel_block(renderer, x, y, 1, 7, 1, 1, scale);
    draw_pixel_block(renderer, x, y, 0, 9, 1, 1, scale);
}