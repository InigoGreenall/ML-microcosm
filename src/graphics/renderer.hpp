#include <cstdint>
typedef struct state_t state_t;

state_t* init(uint32_t width, uint32_t height);
bool dispatch_events(state_t* state);
void request_new_frame(state_t* state);
// void disconnect(state_t* state);