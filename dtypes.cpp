#include "main.hpp"
#include "dtypes.hpp"
#include <cmath>
#include <vector>

Vec2::Vec2(float x, float y) :
	x(x),
	y(y)
{}

Vec2::Vec2() :
	x(0),
	y(0)
{}

Vec2 Vec2::operator+=(const Vec2& v) {
	this->x += v.x;
	this->y += v.y;
	return *this;
}

Vec2 Vec2::operator+(const Vec2& v) const {
	return Vec2(x+v.x, y+v.y);
}

Vec2 Vec2::operator*(float f) const {
	return Vec2(x*f, y*f);
}

float Vec2::length() const {
	return std::sqrt(x*x + y*y);
}

Vec2 Vec2::normalise() const {
	float mag = length();

	if(mag == 0) {
		return Vec2(0,0);
	} else {
		return Vec2(x/mag, y/mag);
	}

}

//######################################################
// Entity
Entity::Entity(int x, int y, int size, float fov, float max_accel, float energy_capacity, Net model) :
	x(x),
	y(y),
	size(size),
	fov(fov),
	max_accel(max_accel),
	energy_capacity(energy_capacity),
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
	velocity(0,0),
	acceleration(0,0),
	model(Net())
{}

Entity::~Entity() {}

void Entity::update_velocity() {
	velocity += acceleration;

	float mag = velocity.length();
	float drag = pow(mag,2)*DRAG_COEFF/(float)size; // density == 1

	mag -= drag;
	Vec2 dir = velocity.normalise();
	velocity = dir*mag;
}

Vec2 Entity::query_model() {
	std::vector<float> result = model.make_prediction({(float)x, (float)y, (float)size, max_accel, energy_capacity, velocity.x, velocity.y});
	return Vec2(result[0], result[1]);
}


//######################################################
// EntityMap
EntityMap::EntityMap() {}


void EntityMap::do_tick() {
	// 1. Handle Collisions
		// TODO: Boundary collisions
		// TODO: Handle eating

	// 2. Update FOV raycasts
		// TODO: Entity::raycasts (std::Array<float, N>)
		// TODO: Entity::update_raycasts()
	
	// 3. Run ML Nets (generate "acceleration")
		// DONE: install libtorch
		// DONE: copy model.py into cpp
		// DONE: Vec2 Entity::query_model()
	
	// 4. Update values from Nets
		// a. update velocity based on acceleration

		// TODO: 
	
	// 5. Update positions
		// TODO: void EntityMap::update_positions()

}

void EntityMap::update_collision_grid() {
	for (std::vector<Entity*>& v : collision_grid) {
		v.clear();
	}

	for(Entity* e : entities) {
		collision_grid.at((e->x / 8)+10*(e->y / 8)).push_back(e);
	}
}

void EntityMap::check_collisions() {
	for (std::vector<Entity*>& v : collision_grid) {
		for (int i = 0; i < v.size(); i++) {
			for (int j = i+1; j < v.size(); j++) {
				Entity* e1 = v.at(i);
				Entity* e2 = v.at(j);

				if (std::abs(e1->x - e2->x) < (e1->size + e2->size) && std::abs(e1->y - e2->y) < (e1->size + e2->size)) {
					handle_collision(e1, e2);
				}
			}
		}
	}
}
void EntityMap::handle_collision(Entity* e1, Entity* e2) {	
	// find normal vector between the entities
	Vec2 d(e2->x - e1->x, e2->y - e1->y);
	float dist = d.length();
	Vec2 normal = d.normalise();

	// relative velocity along the normal
	float vel_norm = (e1->velocity.x - e2->velocity.x) * normal.x + (e1->velocity.y - e2->velocity.y) * normal.y;

	// prevent triggering collision if entities are moving apart (e.g. collision already occured on previous tick)
	if (vel_norm > 0) {
		return;
	}

	// impulse
	// -(1 + restitution)*vel_norm; resititution is 1 for elastic collision
	float j = -2*vel_norm / (1.0/e1->size + 1.0/e2->size);

	Vec2 I = normal * j;

	//theres some complicated signage here so if it completely breaks after a single collision, this will be why.
	e1->velocity += I * (1.0/e1->size);
	e2->velocity += I * (-1.0/e2->size);
}