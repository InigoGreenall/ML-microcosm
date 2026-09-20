#include "entity.hpp"
#include "vec2.hpp"
#include "../config.hpp"

Entity::Entity(int x, int y, int size, float fov, float max_accel, float energy_capacity, uint32_t display_colour, Net model) :
	x(x),
	y(y),
	size(size),
	fov(fov),
	max_accel(max_accel),
	energy_capacity(energy_capacity),
	display_colour(display_colour),
	velocity(0,0),
	acceleration(0,0),
	model(model)
{}

// default constructor for testing
Entity::Entity() :
	x(WIDTH/2),
	y(HEIGHT/2),
	size(5),
	fov(30),
	max_accel(20),
	energy_capacity(200),
	display_colour(0xFFFF0000),
	velocity(0,0),
	acceleration(0,0),
	model(Net())
{}

Entity::~Entity() {}

Vec2 Entity::query_model() {
	std::vector<float> result = model.get_prediction({(float)x, (float)y, (float)size, max_accel, energy_capacity, velocity.x, velocity.y});
	Vec2 accel = Vec2(result[0], result[1]);
	if (accel.length() > this->max_accel) {
		return accel.normalise() * max_accel;
	}
	else {
		return accel;
	}
}