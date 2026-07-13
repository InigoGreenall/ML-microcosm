#ifndef MAIN_HPP
#define MAIN_HPP

#define TORCH_CPU_ID 0
#define TORCH_CUDA_ID 1

#define TPS 5

#define WIDTH 1024
#define HEIGHT 1024
#define MAX_ENTITIES 65536

#define DRAG_COEFF 0.0002159 //0.47 (sphere drag coefficient) * 3/8 * 0.001225 (fluid density) 

// food blob max
#define FOOD_BLOB_MAX 1000

// food blob spawn rate per tick
#define FOOD_BLOB_SPAWN_RATE 1

#endif // MAIN_HPP