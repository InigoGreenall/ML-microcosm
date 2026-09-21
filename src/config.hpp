#ifndef CONFIG_HPP
#define CONFIG_HPP

constexpr int WIDTH = 800;
constexpr int HEIGHT = 800;

constexpr int NUM_TICKS_PER_UNIT = 20;  // controls physics tick fineness
constexpr int NUM_UNITS_PER_SEC = 2;    // controls actual play speed - only used in main.cpp
constexpr int MAX_ENTITIES = 65536;

#endif