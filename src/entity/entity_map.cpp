#include "entity_map.hpp"
#include "entity.hpp"

EntityMap::EntityMap() {}

void EntityMap::do_tick() {

	// 1 (real). spawn food blobs
	spawn_food_blobs();

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
		e->x += e->velocity.x / NUM_TICKS_PER_UNIT;
		e->y += e->velocity.y / NUM_TICKS_PER_UNIT;
		this->check_and_fix_boundary_collisions(e);
	}	
	
	// 4. Handle Collisions
	this->update_collision_grid();
	this->check_collisions();
}

uint8_t num_height_divisions = 1;
uint8_t num_width_divisions = 1;
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
			/* Check for food blob eating */
			int left_x = std::max(0, e1->x - e1->size);
			int right_x = std::min(WIDTH-1, e1->x + e1->size);
			int upper_y = std::max(0, e1->y - e1->size);
			int bottom_y = std::min(HEIGHT-1, e1->y + e1->size);
			for (int x = left_x; x <= right_x; x++) {
				for (int y = upper_y; y <= bottom_y; y++) {
					if (this->food_blobs[x][y] > 0) {
						if (Vec2(e1->x, e1->y).distance(Vec2(x,y)) < e1->size/(float)2) {
							e1->size += this->food_blobs[x][y];
							this->food_blobs[x][y] = 0;
							this->food_blob_count--;
						}
					}
				} 
			}
			for (int j = i+1; j < v.size(); j++) {
				Entity* e2 = v.at(j);
				int e2_index = std::find(entities.begin(), entities.end(), e2) - entities.begin();
				/* Check for entity eating */
				if (e1->size >= e2->size * 1.3) { // e1 is larger
					if (Vec2(e1->x, e1->y).distance(Vec2(e2->x, e2->y)) <= e1->size) {
						if (!delete_flag[e2_index]) {
							e1->size += e2->size/8;
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
				/* Check for entity collisions */
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

/*	Spawn food blobs randomly throughout the simulation. */
void EntityMap::spawn_food_blobs() {
	int spawn_count = std::min(FOOD_BLOB_SPAWN_RATE, FOOD_BLOB_MAX - this->food_blob_count);
	for (int i = 0; i < spawn_count; i++) {
		int x = std::rand() % WIDTH;
		int y = std::rand() % HEIGHT;
		this->food_blobs[x][y] = std::rand() % 4; // random energy value between 0 and 3
		this->food_blob_count++;
	}
}