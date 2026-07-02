#ifndef TYPES_H
#define TYPES_H

#include "config.h"

typedef struct
{
    char name[50];

    int red;
    int green;
    int blue;

    int alive_count;

    float min_speed;
    float max_speed;
    float base_hp;
    float hunger;
    float aggressiveness;
    float robustness;
} Species;

typedef struct
{
    Species *species;

    float x;
    float y;

    float direction_x;
    float direction_y;

    float hp;
    float max_hp;

    int alive;
    float speed;
    int age;

    int reproduction_cooldown;
    int direction_timer;
} Individual;

typedef struct
{
    Species *species_list[MAX_SPECIES];
    int species_count;

    Individual *individual_list[MAX_INDIVIDUALS];
    int individual_count;

    int step;
    int paused;
    int step_by_step;
    int simulation_speed;

    Individual *selected_individual;

    float food_x[MAX_FOOD_SOURCES];
    float food_y[MAX_FOOD_SOURCES];
    int food_active[MAX_FOOD_SOURCES];
    int food_count;
} Simulation;

#endif