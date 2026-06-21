#include "main.hpp"
#include "dtypes.hpp"
#include "softwarerender.hpp"
extern "C" {
    #define OLIVEC_IMPLEMENTATION
    #include "testing-include/olive.c"
}

extern EntityMap* entity_map;

void frame_drawer(void* pixel_buffer) {
    Olivec_Canvas oc = olivec_canvas((uint32_t*) pixel_buffer, WIDTH, HEIGHT, WIDTH);
    olivec_fill(oc, 0xFFFFFFFF);

    for (unsigned short i = 0; i < entity_map->entities.size(); i++) {
        olivec_circle(
            oc, 
            entity_map->entities[i]->x, 
            entity_map->entities[i]->y, 
            entity_map->entities[i]->size, 
            0xFFFF0000
        );
    }
}