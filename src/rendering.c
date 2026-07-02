#include <stdio.h>
#include <string.h>

#include "rendering.h"
#include "pixel_art.h"
#include "config.h"

static void draw_text(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
    int x,
    int y,
    SDL_Color color
)
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect destination;
    int width;
    int height;

    surface = TTF_RenderUTF8_Blended(font, text, color);

    if (surface == NULL)
    {
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture == NULL)
    {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    destination.x = x;
    destination.y = y;
    destination.w = width;
    destination.h = height;

    SDL_RenderCopy(renderer, texture, NULL, &destination);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void draw_individual(SDL_Renderer *renderer, Individual *individual)
{
    if (individual == NULL || individual->alive == 0)
    {
        return;
    }

    if (strcmp(individual->species->name, "Tarnished") == 0)
    {
        draw_tarnished(renderer, (int)individual->x, (int)individual->y);
    }
    else if (strcmp(individual->species->name, "Misbegotten") == 0)
    {
        draw_misbegotten(renderer, (int)individual->x, (int)individual->y);
    }
    else
    {
        draw_demigod(renderer, (int)individual->x, (int)individual->y);
    }
}

void draw_simulation_entities(SDL_Renderer *renderer, Simulation *simulation)
{
    int i;

    for (i = 0; i < simulation->food_count; i++)
    {
        if (simulation->food_active[i] == 1)
        {
            draw_food(
                renderer,
                (int)simulation->food_x[i],
                (int)simulation->food_y[i]
            );
        }
    }

    for (i = 0; i < simulation->individual_count; i++)
    {
        draw_individual(renderer, simulation->individual_list[i]);
    }
}

void draw_selected_individual_stats(
    SDL_Renderer *renderer,
    Individual *individual,
    TTF_Font *font
)
{
    SDL_Rect panel;
    SDL_Color white;
    char text[200];
    int y;

    if (individual == NULL)
    {
        return;
    }

    panel.x = 250;
    panel.y = 120;
    panel.w = 350;
    panel.h = 290;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 220);
    SDL_RenderFillRect(renderer, &panel);

    white.r = 255;
    white.g = 255;
    white.b = 255;
    white.a = 255;

    y = 150;

    snprintf(text, sizeof(text), "Species: %s", individual->species->name);
    draw_text(renderer, font, text, 280, y, white);
    y += 35;

    snprintf(text, sizeof(text), "HP: %.1f / %.1f", individual->hp, individual->max_hp);
    draw_text(renderer, font, text, 280, y, white);
    y += 35;

    snprintf(text, sizeof(text), "Speed: %.2f", individual->speed);
    draw_text(renderer, font, text, 280, y, white);
    y += 35;

    snprintf(text, sizeof(text), "Age: %d", individual->age);
    draw_text(renderer, font, text, 280, y, white);
    y += 35;

    snprintf(text, sizeof(text), "Hunger: %.2f", individual->species->hunger);
    draw_text(renderer, font, text, 280, y, white);
    y += 35;

    snprintf(text, sizeof(text), "Robustness: %.2f", individual->species->robustness);
    draw_text(renderer, font, text, 280, y, white);
}

void draw_inventory_panel(
    SDL_Renderer *renderer,
    Simulation *simulation,
    TTF_Font *font,
    SDL_Texture *inventory_background
)
{
    int i;
    int j;
    char text[100];

    Species *temporary;
    Species *sorted_species[MAX_SPECIES];

    SDL_Rect background;
    SDL_Color white;

    white.r = 255;
    white.g = 255;
    white.b = 255;
    white.a = 255;

    background.x = SIMULATION_WIDTH;
    background.y = 0;
    background.w = INVENTORY_WIDTH;
    background.h = WINDOW_HEIGHT;

    SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255);
    SDL_RenderFillRect(renderer, &background);

    if (inventory_background != NULL)
    {
        SDL_Rect inventory_destination;

        inventory_destination.x = SIMULATION_WIDTH;
        inventory_destination.y = 0;
        inventory_destination.w = INVENTORY_WIDTH;
        inventory_destination.h = WINDOW_HEIGHT;

        SDL_RenderCopy(renderer, inventory_background, NULL, &inventory_destination);
    }

    for (i = 0; i < simulation->species_count; i++)
    {
        sorted_species[i] = simulation->species_list[i];
    }

    for (i = 0; i < simulation->species_count - 1; i++)
    {
        for (j = i + 1; j < simulation->species_count; j++)
        {
            if (sorted_species[j]->alive_count > sorted_species[i]->alive_count)
            {
                temporary = sorted_species[i];
                sorted_species[i] = sorted_species[j];
                sorted_species[j] = temporary;
            }
        }
    }

    draw_text(renderer, font, "Life Simulation", SIMULATION_WIDTH + 10, 15, white);

    snprintf(text, sizeof(text), "Step: %d", simulation->step);
    draw_text(renderer, font, text, SIMULATION_WIDTH + 10, 45, white);

    snprintf(text, sizeof(text), "Speed: x%d", simulation->simulation_speed);
    draw_text(renderer, font, text, SIMULATION_WIDTH + 10, 70, white);

    for (i = 0; i < simulation->species_count; i++)
    {
        int sprite_x;
        int sprite_y;
        int text_x;
        int text_y;

        sprite_x = SIMULATION_WIDTH + 10;
        sprite_y = 110 + i * 80;

        text_x = SIMULATION_WIDTH + 45;
        text_y = 110 + i * 80 + 10;

        if (strcmp(sorted_species[i]->name, "Tarnished") == 0)
        {
            draw_tarnished(renderer, sprite_x, sprite_y);
        }
        else if (strcmp(sorted_species[i]->name, "Misbegotten") == 0)
        {
            draw_misbegotten(renderer, sprite_x, sprite_y);
        }
        else
        {
            draw_demigod(renderer, sprite_x, sprite_y);
        }

        snprintf(
            text,
            sizeof(text),
            "%s: %d",
            sorted_species[i]->name,
            sorted_species[i]->alive_count
        );

        draw_text(renderer, font, text, text_x, text_y, white);
    }

    draw_text(
        renderer,
        font,
        "Click an individual",
        SIMULATION_WIDTH + 10,
        380,
        white
    );

    draw_text(
        renderer,
        font,
        "to view its stats.",
        SIMULATION_WIDTH + 10,
        400,
        white
    );

    draw_text(renderer, font, "Controls:", SIMULATION_WIDTH + 10, 510, white);
    draw_text(renderer, font, "SPACE - pause", SIMULATION_WIDTH + 10, 535, white);
    draw_text(renderer, font, "ENTER - step mode", SIMULATION_WIDTH + 10, 560, white);
    draw_text(renderer, font, "Z - speed up", SIMULATION_WIDTH + 10, 585, white);
    draw_text(renderer, font, "A - slow down", SIMULATION_WIDTH + 10, 610, white);
}