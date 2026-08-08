#include "main.hpp"
#include "dtypes.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <vector>

//###################################################
// Vec2
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

float Vec2::distance(Vec2 v) {
	return std::sqrt((x-v.x)*(x-v.x) + (y-v.y)*(y-v.y));
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
	velocity.x += acceleration.x / NUM_TICKS_PER_SEC;
	velocity.y += acceleration.y / NUM_TICKS_PER_SEC;

	float mag = velocity.length();
	float drag = pow(mag,2)*DRAG_COEFF/(float)size; // density == 1

	mag -= drag;
	Vec2 dir = velocity.normalise();
	velocity = dir*mag;
}

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


//######################################################
// EntityMap
EntityMap::EntityMap() {}


void EntityMap::do_tick() {
	// 1. Update FOV raycasts
	// TODO: Entity::raycasts (std::Array<float, N>)
	// TODO: Entity::update_raycasts()
	
	// 2. Run Network prediction (entity "decides" acceleration)
	for (Entity* e : this->entities) {
		e->acceleration = e->query_model() * ((float)1/2);
		e->update_velocity();
	}
	
	// 3. Update positions
	for (Entity* e : this->entities) {
		e->x += e->velocity.x / NUM_TICKS_PER_SEC;
		e->y += e->velocity.y / NUM_TICKS_PER_SEC;
		this->check_and_fix_boundary_collisions(e);
	}	
	
	// 4. Handle Collisions
	this->update_collision_grid();
	this->check_collisions();
}

uint8_t num_height_divisions = 8;
uint8_t num_width_divisions = 8;
void EntityMap::update_collision_grid() {
	for (std::vector<Entity*>& v : collision_grid) {
		v.clear();
	}
	
	for(Entity* e : entities) {
		// collision_grid.at(((float)e->x / WIDTH * 8) + 8 * ((float)e->y / HEIGHT * 8)).push_back(e);
		uint8_t grid_x = std::floor(num_width_divisions * ((float)e->x / (WIDTH+1)));
		uint8_t grid_y = std::floor(num_height_divisions * ((float)e->y / (HEIGHT+1)));
		assert(grid_x < num_width_divisions && "grid_x exceeds width divisions");
		assert(grid_y < num_height_divisions && "grid_y exceeds height divisions");
		collision_grid[grid_x + num_width_divisions * grid_y].push_back(e);
	}
}

void EntityMap::check_collisions() {
	uint8_t delete_flag[entities.size()];
	std::memset(delete_flag, 0, entities.size());
	for (std::vector<Entity*>& v : collision_grid) {
		for (int i = 0; i < v.size(); i++) {
			Entity* e1 = v.at(i);
			int e1_index = std::find(entities.begin(), entities.end(), e1) - entities.begin();
			for (int j = i+1; j < v.size(); j++) {
				Entity* e2 = v.at(j);
				int e2_index = std::find(entities.begin(), entities.end(), e2) - entities.begin();
				// check for eating
				if (e1->size >= e2->size * 1.3) { // e1 is larger
					if (Vec2(e1->x, e1->y).distance(Vec2(e2->x, e2->y)) <= e1->size) {
						if (!delete_flag[e2_index]) {
							e1->size += e2->size/4;
							delete_flag[e2_index] = true;
						}
					}
				}
				else if (e2->size >= e1->size * 1.3) { // e2 is larger
					if (Vec2(e2->x, e2->y).distance(Vec2(e1->x, e1->y)) <= e2->size) {
						if (!delete_flag[e1_index]) {
							e2->size += e1->size/4;
							delete_flag[e1_index] = true;
						}
					}
				}
				// check for entity collisions
				else if (std::abs(e1->x - e2->x) < (e1->size + e2->size) && std::abs(e1->y - e2->y) < (e1->size + e2->size)) {
					if (!delete_flag[e1_index] && !delete_flag[e2_index]) {
						handle_collision(e1, e2);
					}
				}
			}
		}
	}
	// sweep entities flagged for deletion -- creates new entity vector
	std::vector<Entity*> new_list;
	for (int i = 0; i < entities.size(); i++) {
		if (delete_flag[i]) {
			delete entities[i];
		}
		else {
			new_list.push_back(entities[i]);
		}
	}
	this->entities = new_list;
}

/*	Handle a bounce elastic collision between two entities. */
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
	float j = -1.5*vel_norm / (1.0/e1->size + 1.0/e2->size);

	Vec2 I = normal * j;

	//theres some complicated signage here so if it completely breaks after a single collision, this will be why.
	e1->velocity += I * (-1.0/e1->size);
	e2->velocity += I * (1.0/e2->size);
}

/*  Apply fully elastic bounce entities colliding with the boundary; otherwise is a no-op.
	Postcondition: entity e is within the boundaries of the simulation.
*/
void EntityMap::check_and_fix_boundary_collisions(Entity* e) {
	// check for boundary collisions & apply fully elastic bounce
	if (e->x + e->size > WIDTH) {
		e->velocity.x = -e->velocity.x;
		e->x = WIDTH - e->size;
	}
	else if (e->x - e->size < 0) {
		e->velocity.x = -e->velocity.x;
		e->x = e->size;
	}
	if (e->y + e->size > HEIGHT) {
		e->velocity.y = -e->velocity.y;
		e->y = HEIGHT - e->size;
	}
	else if (e->y - e->size < 0) {
		e->velocity.y = -e->velocity.y;
		e->y = e->size;
	}
}