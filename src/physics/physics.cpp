#include "physics.hpp"
#include "../entity/entity.hpp"
#include "../entity/entity_map.hpp"
#include "../config.hpp"

void Entity::update_velocity() {
	velocity.x += std::ceil(acceleration.x / NUM_TICKS_PER_UNIT);
	velocity.y += std::ceil(acceleration.y / NUM_TICKS_PER_UNIT);

	float mag = velocity.length();
	float drag = pow(mag,2)*DRAG_COEFF/(float)size; // density == 1

	mag -= drag;
	Vec2 dir = velocity.normalise();
	velocity = dir*mag;
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
	float j = -(1 + RESTITUTION) * vel_norm / (1.0/e1->size + 1.0/e2->size);

	Vec2 I = normal * j;

	//theres some complicated signage here so if it completely breaks after a single collision, this will be why.
	e1->velocity += I * (-1.0/e1->size) * (1.0 / NUM_TICKS_PER_UNIT);
	e2->velocity += I * (1.0/e2->size) * (1.0 / NUM_TICKS_PER_UNIT);
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