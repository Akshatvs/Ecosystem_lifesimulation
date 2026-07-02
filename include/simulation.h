#ifndef SIMULATION_H
#define SIMULATION_H

#include "types.h"

float random_float(float min, float max);

Species *create_species(
    const char *name,
    int red,
    int green,
    int blue,
    float min_speed,
    float max_speed,
    float base_hp,
    float hunger,
    float aggressiveness,
    float robustness
);

Individual *create_individual(Species *species);

void add_individual(Simulation *simulation, Individual *individual);
void count_alive_individuals(Simulation *simulation);

void init_simulation(Simulation *simulation);
void free_simulation(Simulation *simulation);

void move_individuals(Simulation *simulation, float dt);
void apply_hunger(Simulation *simulation, float dt);

float squared_distance(Individual *a, Individual *b);

int can_eat(Individual *predator, Individual *prey);

void eat_individuals(Simulation *simulation);
void eat_food(Simulation *simulation);

void age_individuals(Simulation *simulation, float dt);
void reproduce_individuals(Simulation *simulation);
void remove_dead_individuals(Simulation *simulation);

void update_simulation(Simulation *simulation, float dt);
void reset_simulation_population(
    Simulation *simulation,
    int tarnished_count,
    int misbegotten_count,
    int demigod_count
);

#endif