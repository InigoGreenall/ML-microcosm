#ifndef ENTITY_MAP_HPP
#define ENTITY_MAP_HPP

#include "entity.hpp"
#include "../config.hpp"

#define FOOD_BLOB_MAX 1000
// food blob spawn rate per tick
#define FOOD_BLOB_SPAWN_RATE 1

class EntityMap {
    public:
		EntityMap();
        ~EntityMap();

        std::vector<Entity*> entities;
        std::array<std::vector<Entity*>, 64> collision_grid; //divided into 64 subsections

        std::array<std::array<int, WIDTH>, HEIGHT> food_blobs; // 2D array of food blob energy values
        int food_blob_count = 0; // current number of food blobs in the simulation

        void do_tick();
        void update_collision_grid();
        void check_collisions();
        void check_and_fix_boundary_collisions(Entity* e);
        void handle_collision(Entity* e1, Entity* e2);
        void spawn_food_blobs();
};

#endif