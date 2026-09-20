#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstdint>
#include "vec2.hpp"
#include "../model/model.hpp"
class Entity {
    public:
        int x;
        int y;
        int size;
        float fov;
        float max_accel;
        float energy_capacity;

        uint32_t display_colour;
		
		Vec2 velocity;
        Vec2 acceleration;

        Net model;
		
        Entity(int x, int y, int size, float fov, float max_accel, float energy_capacity, uint32_t, Net model);
        Entity();

        ~Entity();
		
		void update_velocity();
        Vec2 query_model();
};

#endif