#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <cstddef>
#include "renderer.hpp"
#include "../entity/entity_map.hpp"
extern "C" {
    #define OLIVEC_IMPLEMENTATION
    #include "../../vendor/olive.c"
}

extern EntityMap* entity_map;

struct state_t {
    uint32_t width;
    uint32_t height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    Olivec_Canvas oc;
    uint32_t* pixel_buffer;
};

state_t* init(uint32_t width, uint32_t height) {
    state_t* state = new state_t {
        .width = width,
        .height = height,
    };
    state->pixel_buffer = new uint32_t[width * height];
    SDL_Init(SDL_INIT_VIDEO);
    state->window = SDL_CreateWindow("ML-microcosm", width, height, 0);
    state->renderer = SDL_CreateRenderer(state->window, NULL);
    state->texture = SDL_CreateTexture(state->renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    state->oc  = olivec_canvas((uint32_t*) state->pixel_buffer, width, height, width);
    return state;
}

void request_new_frame(state_t* state) {
    olivec_fill(state->oc, 0xFFFFFFFF);
    
    for (unsigned short i = 0; i < entity_map->entities.size(); i++) {
        olivec_circle(
            state->oc, 
            entity_map->entities[i]->x, 
            entity_map->entities[i]->y, 
            entity_map->entities[i]->size, 
            entity_map->entities[i]->display_colour
        );
    }
    for (unsigned short i = 0; i < entity_map->food_blobs.size(); i++) {
        for (unsigned short j = 0; j < entity_map->food_blobs[i].size(); j++) {
            if (entity_map->food_blobs[i][j] > 0) {
                olivec_circle(
                    state->oc, 
                    j, 
                    i, 
                    entity_map->food_blobs[i][j], 
                    0xFF00FF00
                );
            }
        }
    }
    SDL_UpdateTexture(state->texture, NULL, state->pixel_buffer, state->width * sizeof(uint32_t));
    SDL_RenderClear(state->renderer);
    SDL_RenderTexture(state->renderer, state->texture, NULL, NULL);
    SDL_RenderPresent(state->renderer);
}

bool dispatch_events(state_t* state) {
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_EVENT_QUIT) {
        SDL_DestroyTexture(state->texture);
        SDL_DestroyRenderer(state->renderer);
        SDL_DestroyWindow(state->window);
        SDL_Quit();
        return false;
    }
    return true;
}