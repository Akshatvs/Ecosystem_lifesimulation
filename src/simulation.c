#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simulation.h"
#include "config.h"

float random_float(float min, float max)
{
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

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
)
{
    Species *species = malloc(sizeof(Species));

    if (species == NULL)
    {
        printf("Error: failed to allocate memory for species.\n");
        return NULL;
    }

    strncpy(species->name, name, sizeof(species->name) - 1);
    species->name[sizeof(species->name) - 1] = '\0';

    species->red = red;
    species->green = green;
    species->blue = blue;

    species->alive_count = 0;

    species->min_speed = min_speed;
    species->max_speed = max_speed;
    species->base_hp = base_hp;
    species->hunger = hunger;
    species->aggressiveness = aggressiveness;
    species->robustness = robustness;

    return species;
}

Individual *create_individual(Species *species)
{
    Individual *individual = malloc(sizeof(Individual));

    if (individual == NULL)
    {
        printf("Error: failed to allocate memory for individual.\n");
        return NULL;
    }

    individual->species = species;

    individual->x = random_float(0, SIMULATION_WIDTH - SPRITE_SIZE);
    individual->y = random_float(0, SIMULATION_HEIGHT - SPRITE_SIZE);

    individual->direction_x = random_float(-1, 1);
    individual->direction_y = random_float(-1, 1);

    if (individual->direction_x == 0 && individual->direction_y == 0)
    {
        individual->direction_x = 1;
    }

    individual->max_hp = species->base_hp;
    individual->hp = individual->max_hp;

    individual->alive = 1;
    individual->speed = random_float(species->min_speed, species->max_speed);

    individual->age = 0;
    individual->reproduction_cooldown = 0;
    individual->direction_timer = (int)random_float(30, 120);

    return individual;
}

void add_individual(Simulation *simulation, Individual *individual)
{
    if (individual == NULL)
    {
        return;
    }

    if (simulation->individual_count >= MAX_INDIVIDUALS)
    {
        return;
    }

    simulation->individual_list[simulation->individual_count] = individual;
    simulation->individual_count++;

    individual->species->alive_count++;
}

void count_alive_individuals(Simulation *simulation)
{
    int i;

    for (i = 0; i < simulation->species_count; i++)
    {
        simulation->species_list[i]->alive_count = 0;
    }

    for (i = 0; i < simulation->individual_count; i++)
    {
        Individual *individual = simulation->individual_list[i];

        if (individual != NULL && individual->alive == 1)
        {
            individual->species->alive_count++;
        }
    }
}

void init_simulation(Simulation *simulation)
{
    int i;

    simulation->species_count = 0;
    simulation->individual_count = 0;

    simulation->step = 0;
    simulation->paused = 0;
    simulation->step_by_step = 0;
    simulation->simulation_speed = SIMULATION_SPEED_START;

    simulation->selected_individual = NULL;

    simulation->species_list[0] = create_species(
        "Tarnished",
        180,
        160,
        90,
        1.4f,
        2.4f,
        80,
        0.05f,
        0.2f,
        0.4f
    );

    simulation->species_list[1] = create_species(
        "Misbegotten",
        160,
        60,
        40,
        1.0f,
        1.8f,
        120,
        0.08f,
        0.6f,
        0.6f
    );

    simulation->species_list[2] = create_species(
        "Demigod",
        220,
        180,
        40,
        0.6f,
        1.2f,
        220,
        0.03f,
        1.0f,
        1.0f
    );

    if (
        simulation->species_list[0] == NULL ||
        simulation->species_list[1] == NULL ||
        simulation->species_list[2] == NULL
    )
    {
        return;
    }

    simulation->species_count = 3;
    simulation->food_count = 10;

    for (i = 0; i < simulation->food_count; i++)
    {
        simulation->food_x[i] = random_float(0, SIMULATION_WIDTH - SPRITE_SIZE);
        simulation->food_y[i] = random_float(0, SIMULATION_HEIGHT - SPRITE_SIZE);
        simulation->food_active[i] = 1;
    }
}

void free_simulation(Simulation *simulation)
{
    int i;

    if (simulation == NULL)
    {
        return;
    }

    for (i = 0; i < simulation->individual_count; i++)
    {
        if (simulation->individual_list[i] != NULL)
        {
            free(simulation->individual_list[i]);
            simulation->individual_list[i] = NULL;
        }
    }

    for (i = 0; i < simulation->species_count; i++)
    {
        if (simulation->species_list[i] != NULL)
        {
            free(simulation->species_list[i]);
            simulation->species_list[i] = NULL;
        }
    }

    simulation->individual_count = 0;
    simulation->species_count = 0;
    simulation->selected_individual = NULL;
}

void reset_simulation_population(
    Simulation *simulation,
    int tarnished_count,
    int misbegotten_count,
    int demigod_count
)
{
    int i;

    simulation->individual_count = 0;

    for (i = 0; i < tarnished_count; i++)
    {
        add_individual(simulation, create_individual(simulation->species_list[0]));
    }

    for (i = 0; i < misbegotten_count; i++)
    {
        add_individual(simulation, create_individual(simulation->species_list[1]));
    }

    for (i = 0; i < demigod_count; i++)
    {
        add_individual(simulation, create_individual(simulation->species_list[2]));
    }

    count_alive_individuals(simulation);
}

void move_individuals(Simulation *simulation, float dt)
{
    int i;

    for (i = 0; i < simulation->individual_count; i++)
    {
        Individual *individual = simulation->individual_list[i];

        if (individual == NULL || individual->alive == 0)
        {
            continue;
        }

        individual->direction_timer--;

        if (individual->direction_timer <= 0)
        {
            individual->direction_x = random_float(-1, 1);
            individual->direction_y = random_float(-1, 1);

            if (individual->direction_x == 0 && individual->direction_y == 0)
            {
                individual->direction_x = 1;
            }

            individual->direction_timer = (int)random_float(10, 40);
        }

        individual->x += individual->direction_x * individual->speed * dt * TARGET_FPS;
        individual->y += individual->direction_y * individual->speed * dt * TARGET_FPS;

        if (individual->x < 0)
        {
            individual->x = 0;
            individual->direction_x = -individual->direction_x;
        }

        if (individual->x > SIMULATION_WIDTH - SPRITE_SIZE)
        {
            individual->x = SIMULATION_WIDTH - SPRITE_SIZE;
            individual->direction_x = -individual->direction_x;
        }

        if (individual->y < 0)
        {
            individual->y = 0;
            individual->direction_y = -individual->direction_y;
        }

        if (individual->y > SIMULATION_HEIGHT - SPRITE_SIZE)
        {
            individual->y = SIMULATION_HEIGHT - SPRITE_SIZE;
            individual->direction_y = -individual->direction_y;
        }
    }
}

void apply_hunger(Simulation *simulation, float dt)
{
    int i;

    for (i = 0; i < simulation->individual_count; i++)
    {
        Individual *individual = simulation->individual_list[i];

        if (individual == NULL || individual->alive == 0)
        {
            continue;
        }

        individual->hp -= (
            individual->species->hunger /
            individual->species->robustness
        ) * dt * TARGET_FPS;

        if (individual->hp <= 0)
        {
            individual->hp = 0;
            individual->alive = 0;
        }
    }
}

float squared_distance(Individual *a, Individual *b)
{
    float dx = a->x - b->x;
    float dy = a->y - b->y;

    return dx * dx + dy * dy;
}

int can_eat(Individual *predator, Individual *prey)
{
    if (predator == NULL || prey == NULL)
    {
        return 0;
    }

    if (predator == prey)
    {
        return 0;
    }

    if (predator->alive == 0 || prey->alive == 0)
    {
        return 0;
    }

    if (strcmp(predator->species->name, "Misbegotten") == 0)
    {
        if (strcmp(prey->species->name, "Tarnished") == 0)
        {
            return 1;
        }
    }

    if (strcmp(predator->species->name, "Demigod") == 0)
    {
        if (strcmp(prey->species->name, "Misbegotten") == 0)
        {
            return 1;
        }
    }

    return 0;
}

void eat_individuals(Simulation *simulation)
{
    int i;
    int j;

    for (i = 0; i < simulation->individual_count; i++)
    {
        Individual *predator = simulation->individual_list[i];

        if (predator == NULL || predator->alive == 0)
        {
            continue;
        }

        for (j = 0; j < simulation->individual_count; j++)
        {
            Individual *prey = simulation->individual_list[j];

            if (can_eat(predator, prey) == 1)
            {
                if (squared_distance(predator, prey) < COLLISION_DISTANCE_SQUARED)
                {
                    prey->alive = 0;

                    predator->hp += 35;

                    if (predator->hp > predator->max_hp)
                    {
                        predator->hp = predator->max_hp;
                    }

                    break;
                }
            }
        }
    }
}

void eat_food(Simulation *simulation)
{
    int i;
    int food_index;

    for (i = 0; i < simulation->individual_count; i++)
    {
        Individual *individual = simulation->individual_list[i];

        if (individual == NULL || individual->alive == 0)
        {
            continue;
        }

        if (strcmp(individual->species->name, "Tarnished") != 0)
        {
            continue;
        }

        for (food_index = 0; food_index < simulation->food_count; food_index++)
        {
            float dx;
            float dy;
            float distance;

            if (simulation->food_active[food_index] == 0)
            {
                continue;
            }

            dx = individual->x - simulation->food_x[food_index];
            dy = individual->y - simulation->food_y[food_index];
            distance = dx * dx + dy * dy;

            if (distance < COLLISION_DISTANCE_SQUARED)
            {
                simulation->food_active[food_index] = 0;

                individual->hp += 40;

                if (individual->hp > individual->max_hp)
                {
                    individual->hp = individual->max_hp;
                }

                simulation->food_x[food_index] = random_float(
                    0,
                    SIMULATION_WIDTH - SPRITE_SIZE
                );

                simulation->food_y[food_index] = random_float(
                    0,
                    SIMULATION_HEIGHT - SPRITE_SIZE
                );

                simulation->food_active[food_index] = 1;
                break;
            }
        }
    }
}

void age_individuals(Simulation *simulation, float dt)
{
    int i;

    for (i = 0; i < simulation->individual_count; i++)
    {
        Individual *individual = simulation->individual_list[i];

        if (individual == NULL || individual->alive == 0)
        {
            continue;
        }

        individual->age += (int)(dt * TARGET_FPS);

        if (individual->reproduction_cooldown > 0)
        {
            individual->reproduction_cooldown -= (int)(dt * TARGET_FPS);

            if (individual->reproduction_cooldown < 0)
            {
                individual->reproduction_cooldown = 0;
            }
        }
    }
}

void reproduce_individuals(Simulation *simulation)
{
    int i;
    int j;

    for (i = 0; i < simulation->individual_count; i++)
    {
        Individual *parent_a = simulation->individual_list[i];

        if (parent_a == NULL || parent_a->alive == 0)
        {
            continue;
        }

        if (
            parent_a->age < MIN_REPRODUCTION_AGE ||
            parent_a->reproduction_cooldown > 0
        )
        {
            continue;
        }

        for (j = i + 1; j < simulation->individual_count; j++)
        {
            Individual *parent_b = simulation->individual_list[j];
            Individual *child;

            if (parent_b == NULL || parent_b->alive == 0)
            {
                continue;
            }

            if (strcmp(parent_a->species->name, parent_b->species->name) != 0)
            {
                continue;
            }

            if (
                parent_b->age < MIN_REPRODUCTION_AGE ||
                parent_b->reproduction_cooldown > 0
            )
            {
                continue;
            }

            if (squared_distance(parent_a, parent_b) < COLLISION_DISTANCE_SQUARED)
            {
                if (simulation->individual_count >= MAX_INDIVIDUALS)
                {
                    break;
                }

                child = create_individual(parent_a->species);

                if (child == NULL)
                {
                    break;
                }

                child->speed = (
                    parent_a->speed +
                    parent_b->speed
                ) / 2.0f + random_float(-0.2f, 0.2f);

                child->max_hp = (
                    parent_a->max_hp +
                    parent_b->max_hp
                ) / 2.0f + random_float(-5.0f, 5.0f);

                child->hp = child->max_hp;

                if (child->speed < child->species->min_speed)
                {
                    child->speed = child->species->min_speed;
                }

                if (child->speed > child->species->max_speed)
                {
                    child->speed = child->species->max_speed;
                }

                if (child->max_hp < 20)
                {
                    child->max_hp = 20;
                }

                if (child->max_hp > 500)
                {
                    child->max_hp = 500;
                }

                child->x = (parent_a->x + parent_b->x) / 2.0f;
                child->y = (parent_a->y + parent_b->y) / 2.0f;

                parent_a->reproduction_cooldown = REPRODUCTION_COOLDOWN;
                parent_b->reproduction_cooldown = REPRODUCTION_COOLDOWN;

                add_individual(simulation, child);
                break;
            }
        }
    }
}

void remove_dead_individuals(Simulation *simulation)
{
    int i;

    for (i = 0; i < simulation->individual_count; i++)
    {
        Individual *individual = simulation->individual_list[i];

        if (individual != NULL && individual->alive == 0)
        {
            int j;

            if (individual == simulation->selected_individual)
            {
                simulation->selected_individual = NULL;
            }

            free(individual);

            for (j = i; j < simulation->individual_count - 1; j++)
            {
                simulation->individual_list[j] = simulation->individual_list[j + 1];
            }

            simulation->individual_list[simulation->individual_count - 1] = NULL;
            simulation->individual_count--;
            i--;
        }
    }
}

void update_simulation(Simulation *simulation, float dt)
{
    move_individuals(simulation, dt);
    apply_hunger(simulation, dt);
    eat_individuals(simulation);
    eat_food(simulation);
    remove_dead_individuals(simulation);
    age_individuals(simulation, dt);
    reproduce_individuals(simulation);
    count_alive_individuals(simulation);

    simulation->step++;
}