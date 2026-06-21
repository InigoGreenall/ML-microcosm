struct state_t* init(int width, int height);
void install_frame_drawer(struct state_t* state, void (*frame_drawer)(void* pixel_buffer));
void request_new_frame(struct state_t* state);
int get_last_frame_time_ms(struct state_t* state);
int dispatch_events(struct state_t* state);
void disconnect(struct state_t* state);