#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "config.h"
#include "simulation.h"
#include "rendering.h"

#define FONT_PATH "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"

void draw_menu_text(
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

void draw_centered_text(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
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

    destination.x = WINDOW_WIDTH / 2 - width / 2;
    destination.y = y;
    destination.w = width;
    destination.h = height;

    SDL_RenderCopy(renderer, texture, NULL, &destination);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

SDL_Texture *load_texture(SDL_Renderer *renderer, const char *path)
{
    SDL_Surface *surface;
    SDL_Texture *texture;

    surface = IMG_Load(path);

    if (surface == NULL)
    {
        printf("Failed to load image %s: %s\n", path, IMG_GetError());
        return NULL;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (texture == NULL)
    {
        printf("Failed to create texture %s: %s\n", path, SDL_GetError());
        return NULL;
    }

    return texture;
}

void draw_cover_background(SDL_Renderer *renderer, SDL_Texture *texture)
{
    int image_width;
    int image_height;
    float ratio_x;
    float ratio_y;
    float ratio;
    SDL_Rect destination;

    if (texture == NULL)
    {
        return;
    }

    SDL_QueryTexture(texture, NULL, NULL, &image_width, &image_height);

    ratio_x = (float)WINDOW_WIDTH / image_width;
    ratio_y = (float)WINDOW_HEIGHT / image_height;

    if (ratio_x > ratio_y)
    {
        ratio = ratio_x;
    }
    else
    {
        ratio = ratio_y;
    }

    destination.w = (int)(image_width * ratio);
    destination.h = (int)(image_height * ratio);
    destination.x = (WINDOW_WIDTH - destination.w) / 2;
    destination.y = (WINDOW_HEIGHT - destination.h) / 2;

    SDL_RenderCopy(renderer, texture, NULL, &destination);
}

void draw_dark_overlay(SDL_Renderer *renderer)
{
    SDL_Rect overlay;

    overlay.x = 0;
    overlay.y = 0;
    overlay.w = WINDOW_WIDTH;
    overlay.h = WINDOW_HEIGHT;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
    SDL_RenderFillRect(renderer, &overlay);
}

void draw_main_menu(
    SDL_Renderer *renderer,
    TTF_Font *font,
    TTF_Font *title_font
)
{
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color gold = {220, 190, 120, 255};

    draw_centered_text(renderer, title_font, "NIGHTREIGN", 90, gold);
    draw_centered_text(renderer, font, "[ ENTER ]   Start simulation", 420, white);
    draw_centered_text(renderer, font, "[ C ]   Controls and settings", 470, white);
    draw_centered_text(renderer, font, "[ ESC ]   Quit", 520, white);
}

void draw_settings_menu(
    SDL_Renderer *renderer,
    TTF_Font *font,
    TTF_Font *title_font,
    int selected_setting,
    int tarnished_count,
    int misbegotten_count,
    int demigod_count,
    int start_speed,
    int biome
)
{
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color gold = {220, 190, 120, 255};
    char text[100];
    const char *biome_name;

    draw_centered_text(renderer, title_font, "SETTINGS", 80, gold);

    snprintf(text, sizeof(text), "Tarnished: %d", tarnished_count);
    draw_menu_text(renderer, font, text, 420, 240, selected_setting == 0 ? gold : white);

    snprintf(text, sizeof(text), "Misbegotten: %d", misbegotten_count);
    draw_menu_text(renderer, font, text, 420, 290, selected_setting == 1 ? gold : white);

    snprintf(text, sizeof(text), "Demigod: %d", demigod_count);
    draw_menu_text(renderer, font, text, 420, 340, selected_setting == 2 ? gold : white);

    snprintf(text, sizeof(text), "Simulation speed: %d", start_speed);
    draw_menu_text(renderer, font, text, 420, 390, selected_setting == 3 ? gold : white);

    if (biome == 0)
    {
        biome_name = "Catacombs";
    }
    else
    {
        biome_name = "Oldcastle";
    }

    snprintf(text, sizeof(text), "Biome: %s", biome_name);
    draw_menu_text(renderer, font, text, 420, 440, selected_setting == 4 ? gold : white);

    draw_menu_text(renderer, font, "ARROWS - modify", 420, 560, white);
    draw_menu_text(renderer, font, "ESC - back", 420, 595, white);
}

void draw_pause_menu(
    SDL_Renderer *renderer,
    TTF_Font *font,
    TTF_Font *title_font
)
{
    SDL_Rect panel;
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color gold = {220, 190, 120, 255};

    panel.x = 250;
    panel.y = 150;
    panel.w = 700;
    panel.h = 400;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_RenderFillRect(renderer, &panel);

    draw_centered_text(renderer, title_font, "PAUSED", 190, gold);
    draw_centered_text(renderer, font, "[ R ]   Restart simulation", 330, white);
    draw_centered_text(renderer, font, "[ M ]   Return to main menu", 380, white);
    draw_centered_text(renderer, font, "[ ESC ]   Resume", 430, white);
}

void select_individual_at_mouse(
    Simulation *simulation,
    int mouse_x,
    int mouse_y
)
{
    int i;

    simulation->selected_individual = NULL;

    for (i = 0; i < simulation->individual_count; i++)
    {
        Individual *individual = simulation->individual_list[i];

        if (individual == NULL || individual->alive == 0)
        {
            continue;
        }

        if (
            mouse_x >= individual->x &&
            mouse_x <= individual->x + SPRITE_SIZE &&
            mouse_y >= individual->y &&
            mouse_y <= individual->y + SPRITE_SIZE
        )
        {
            simulation->selected_individual = individual;
            break;
        }
    }
}

Mix_Music *load_biome_music(int biome)
{
    if (biome == 0)
    {
        return Mix_LoadMUS("SONS/Catacombs.mp3");
    }

    return Mix_LoadMUS("SONS/Oldcastle.mp3");
}

int main(void)
{
    Simulation simulation;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;

    SDL_Texture *menu_background = NULL;
    SDL_Texture *catacombs_background = NULL;
    SDL_Texture *oldcastle_background = NULL;
    SDL_Texture *inventory_background = NULL;
    SDL_Texture *current_background = NULL;

    TTF_Font *font = NULL;
    TTF_Font *title_font = NULL;

    Mix_Music *menu_music = NULL;
    Mix_Music *biome_music = NULL;
    Mix_Chunk *selection_sound = NULL;

    int program_running = 1;
    int in_main_menu = 1;
    int in_settings_menu = 0;
    int simulation_running = 0;
    int in_pause_menu = 0;

    int selected_setting = 0;
    int biome = 0;

    int tarnished_count = DEFAULT_TARNISHED_COUNT;
    int misbegotten_count = DEFAULT_MISBEGOTTEN_COUNT;
    int demigod_count = DEFAULT_DEMIGOD_COUNT;
    int start_speed = SIMULATION_SPEED_START;

    Uint32 frame_start;
    Uint32 frame_duration;
    Uint32 previous_time;
    float dt;

    int i;

    srand((unsigned int)time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
    {
        printf("Mix_OpenAudio failed: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0)
    {
        printf("IMG_Init failed: %s\n", IMG_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() != 0)
    {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        IMG_Quit();
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    window = SDL_CreateWindow(
        "2D Ecosystem Simulation",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );

    if (window == NULL)
    {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == NULL)
    {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    menu_background = load_texture(renderer, "IMAGES/NIGHTREIGN.png");
    catacombs_background = load_texture(renderer, "IMAGES/Catacombs.png");
    oldcastle_background = load_texture(renderer, "IMAGES/Oldcastle.png");
    inventory_background = load_texture(renderer, "IMAGES/Inventaire.png");

    if (
        menu_background == NULL ||
        catacombs_background == NULL ||
        oldcastle_background == NULL ||
        inventory_background == NULL
    )
    {
        printf("One or more required images failed to load.\n");
        program_running = 0;
    }

    font = TTF_OpenFont(FONT_PATH, 18);
    title_font = TTF_OpenFont(FONT_PATH, 72);

    if (font == NULL || title_font == NULL)
    {
        printf("Failed to load font: %s\n", TTF_GetError());
        program_running = 0;
    }

    menu_music = Mix_LoadMUS("SONS/menu.mp3");
    selection_sound = Mix_LoadWAV("SONS/selection.mp3");

    if (menu_music != NULL)
    {
        Mix_PlayMusic(menu_music, -1);
    }

    previous_time = SDL_GetTicks();

    while (program_running)
    {
        frame_start = SDL_GetTicks();

        dt = (frame_start - previous_time) / 1000.0f;
        previous_time = frame_start;

        if (dt > 0.05f)
        {
            dt = 0.05f;
        }

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                program_running = 0;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (simulation_running == 1 && in_pause_menu == 0)
                {
                    select_individual_at_mouse(
                        &simulation,
                        event.button.x,
                        event.button.y
                    );
                }
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (in_main_menu == 1)
                {
                    if (event.key.keysym.sym == SDLK_RETURN)
                    {
                        init_simulation(&simulation);

                        reset_simulation_population(
                            &simulation,
                            tarnished_count,
                            misbegotten_count,
                            demigod_count
                        );

                        simulation.simulation_speed = start_speed;

                        Mix_HaltMusic();

                        if (biome_music != NULL)
                        {
                            Mix_FreeMusic(biome_music);
                            biome_music = NULL;
                        }

                        biome_music = load_biome_music(biome);

                        if (biome_music != NULL)
                        {
                            Mix_PlayMusic(biome_music, -1);
                        }

                        simulation_running = 1;
                        in_main_menu = 0;
                    }

                    if (event.key.keysym.sym == SDLK_c)
                    {
                        in_settings_menu = 1;
                        in_main_menu = 0;
                    }

                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        program_running = 0;
                    }
                }
                else if (in_settings_menu == 1)
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        in_settings_menu = 0;
                        in_main_menu = 1;
                    }

                    if (event.key.keysym.sym == SDLK_UP)
                    {
                        if (selected_setting > 0)
                        {
                            selected_setting--;
                        }

                        if (selection_sound != NULL)
                        {
                            Mix_PlayChannel(-1, selection_sound, 0);
                        }
                    }

                    if (event.key.keysym.sym == SDLK_DOWN)
                    {
                        if (selected_setting < 4)
                        {
                            selected_setting++;
                        }

                        if (selection_sound != NULL)
                        {
                            Mix_PlayChannel(-1, selection_sound, 0);
                        }
                    }

                    if (event.key.keysym.sym == SDLK_RIGHT)
                    {
                        if (selected_setting == 0)
                        {
                            tarnished_count += 5;
                        }
                        else if (selected_setting == 1)
                        {
                            misbegotten_count += 5;
                        }
                        else if (selected_setting == 2)
                        {
                            demigod_count += 1;
                        }
                        else if (
                            selected_setting == 3 &&
                            start_speed < SIMULATION_SPEED_MAX
                        )
                        {
                            start_speed++;
                        }
                        else if (selected_setting == 4)
                        {
                            biome = 1 - biome;
                        }

                        if (selection_sound != NULL)
                        {
                            Mix_PlayChannel(-1, selection_sound, 0);
                        }
                    }

                    if (event.key.keysym.sym == SDLK_LEFT)
                    {
                        if (selected_setting == 0 && tarnished_count > 5)
                        {
                            tarnished_count -= 5;
                        }
                        else if (selected_setting == 1 && misbegotten_count > 5)
                        {
                            misbegotten_count -= 5;
                        }
                        else if (selected_setting == 2 && demigod_count > 1)
                        {
                            demigod_count -= 1;
                        }
                        else if (
                            selected_setting == 3 &&
                            start_speed > SIMULATION_SPEED_MIN
                        )
                        {
                            start_speed--;
                        }
                        else if (selected_setting == 4)
                        {
                            biome = 1 - biome;
                        }

                        if (selection_sound != NULL)
                        {
                            Mix_PlayChannel(-1, selection_sound, 0);
                        }
                    }
                }
                else if (simulation_running == 1)
                {
                    if (in_pause_menu == 1)
                    {
                        if (event.key.keysym.sym == SDLK_r)
                        {
                            free_simulation(&simulation);
                            init_simulation(&simulation);

                            reset_simulation_population(
                                &simulation,
                                tarnished_count,
                                misbegotten_count,
                                demigod_count
                            );

                            simulation.simulation_speed = start_speed;
                            simulation.paused = 0;
                            in_pause_menu = 0;
                        }

                        if (event.key.keysym.sym == SDLK_m)
                        {
                            free_simulation(&simulation);

                            Mix_HaltMusic();

                            if (biome_music != NULL)
                            {
                                Mix_FreeMusic(biome_music);
                                biome_music = NULL;
                            }

                            if (menu_music != NULL)
                            {
                                Mix_PlayMusic(menu_music, -1);
                            }

                            simulation_running = 0;
                            in_pause_menu = 0;
                            in_main_menu = 1;
                        }

                        if (event.key.keysym.sym == SDLK_ESCAPE)
                        {
                            in_pause_menu = 0;
                        }
                    }
                    else
                    {
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                        {
                            in_pause_menu = 1;
                        }

                        if (event.key.keysym.sym == SDLK_SPACE)
                        {
                            simulation.paused = 1 - simulation.paused;
                        }

                        if (event.key.keysym.sym == SDLK_RETURN)
                        {
                            simulation.step_by_step = 1;
                        }

                        if (
                            event.key.keysym.sym == SDLK_z &&
                            simulation.simulation_speed < SIMULATION_SPEED_MAX
                        )
                        {
                            simulation.simulation_speed++;
                        }

                        if (
                            event.key.keysym.sym == SDLK_a &&
                            simulation.simulation_speed > SIMULATION_SPEED_MIN
                        )
                        {
                            simulation.simulation_speed--;
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (simulation_running == 1)
        {
            if (biome == 0)
            {
                current_background = catacombs_background;
            }
            else
            {
                current_background = oldcastle_background;
            }
        }
        else
        {
            current_background = menu_background;
        }

        draw_cover_background(renderer, current_background);
        draw_dark_overlay(renderer);

        if (in_main_menu == 1)
        {
            draw_main_menu(renderer, font, title_font);
        }
        else if (in_settings_menu == 1)
        {
            draw_settings_menu(
                renderer,
                font,
                title_font,
                selected_setting,
                tarnished_count,
                misbegotten_count,
                demigod_count,
                start_speed,
                biome
            );
        }
        else if (simulation_running == 1)
        {
            if (simulation.paused == 0 && in_pause_menu == 0)
            {
                for (i = 0; i < simulation.simulation_speed; i++)
                {
                    update_simulation(&simulation, dt);
                }
            }
            else if (simulation.step_by_step == 1 && in_pause_menu == 0)
            {
                update_simulation(&simulation, dt);
                simulation.step_by_step = 0;
            }

            draw_simulation_entities(renderer, &simulation);

            draw_inventory_panel(
                renderer,
                &simulation,
                font,
                inventory_background
            );

            draw_selected_individual_stats(
                renderer,
                simulation.selected_individual,
                font
            );

            if (in_pause_menu == 1)
            {
                draw_pause_menu(renderer, font, title_font);
            }
        }

        SDL_RenderPresent(renderer);

        frame_duration = SDL_GetTicks() - frame_start;

        if (frame_duration < FRAME_DELAY_MS)
        {
            SDL_Delay(FRAME_DELAY_MS - frame_duration);
        }
    }

    if (simulation_running == 1)
    {
        free_simulation(&simulation);
    }

    if (biome_music != NULL)
    {
        Mix_FreeMusic(biome_music);
    }

    if (menu_music != NULL)
    {
        Mix_FreeMusic(menu_music);
    }

    if (selection_sound != NULL)
    {
        Mix_FreeChunk(selection_sound);
    }

    if (font != NULL)
    {
        TTF_CloseFont(font);
    }

    if (title_font != NULL)
    {
        TTF_CloseFont(title_font);
    }

    if (menu_background != NULL)
    {
        SDL_DestroyTexture(menu_background);
    }

    if (catacombs_background != NULL)
    {
        SDL_DestroyTexture(catacombs_background);
    }

    if (oldcastle_background != NULL)
    {
        SDL_DestroyTexture(oldcastle_background);
    }

    if (inventory_background != NULL)
    {
        SDL_DestroyTexture(inventory_background);
    }

    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
    }

    if (window != NULL)
    {
        SDL_DestroyWindow(window);
    }

    TTF_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}