#ifndef PHYSICS_HPP
#define PHYSICS_HPP

// #define DRAG_COEFF 0.0002159 // 0.47 (sphere drag coefficient) * 3/8 * 0.001225 (fluid density) 
// #define DRAG_COEFF 0.0017625 // same formula but replace fluid density with 0.01
#define DRAG_COEFF (0.47 * 3/8 * 0.07)
#define RESTITUTION 1 // for collisions

#endif