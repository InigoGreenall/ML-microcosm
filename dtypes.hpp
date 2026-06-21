#ifndef DTYPES_HPP
#define DTYPES_HPP

#define DRAG_COEFF 0.0002159 //0.47 (sphere drag coefficient) * 3/8 * 0.001225 (fluid density) 

#include <vector>
#include <array>
#include "model.hpp"

struct Vec2 {
    float x, y;
    Vec2(float x, float y);
    Vec2();

    Vec2 operator+=(const Vec2& v);
    Vec2 operator+(const Vec2& v) const;
    Vec2 operator*(float f) const;

    float length() const;
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

        void do_tick();
        void update_collision_grid();
        void check_collisions();
        void handle_collision(Entity* e1, Entity* e2);
};

#endif