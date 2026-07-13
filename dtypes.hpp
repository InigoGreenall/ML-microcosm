#ifndef DTYPES_HPP
#define DTYPES_HPP

#include <vector>
#include <array>
#include "main.hpp"
#include "model.hpp"

struct Vec2 {
    float x, y;
    Vec2(float x, float y);
    Vec2();

    Vec2 operator+=(const Vec2& v);
    Vec2 operator+(const Vec2& v) const;
    Vec2 operator*(float f) const;

    float length() const;
    float distance(Vec2 v);
    Vec2 normalise() const;
};


class Entity {
    public:
        int x;
        int y;
        int size;
        float fov;
        float max_accel;
        float energy_capacity;
		
		Vec2 velocity;
        Vec2 acceleration;

        Net model;
		
        Entity(int x, int y, int size, float fov, float max_accel, float energy_capacity, Net model);
        Entity();

        ~Entity();
		
		void update_velocity();
        Vec2 query_model();
};

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