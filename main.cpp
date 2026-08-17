#include "main.hpp"
#include "dtypes.hpp"
#include "model.hpp"
#include "softwarerender.hpp"
#include <climits>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
extern "C" {
    #include "testing-include/wlclient.h"
}

const int NUM_TICKS_PER_UNIT = 20;  // controls physics tick fineness
const int NUM_UNITS_PER_SEC = 2;    // controls actual play speed - only used in main.cpp
const int WIDTH = 400;
const int HEIGHT = 400;
const int MAX_ENTITIES = 65536;

static const torch::DeviceType DEVICE_LOOKUP[] = {torch::kCPU, torch::kCUDA};

EntityMap* entity_map;

int main() {
    struct state_t* state = init(WIDTH, HEIGHT);
    install_frame_drawer(state, frame_drawer);

    // TODO: do program setup
    entity_map = new EntityMap;
    entity_map->entities.push_back(
        new Entity(WIDTH/2, HEIGHT/2, 30, 1, 1, 1, 0xFFFF0000, Net({7, 64, 64, 2},DEVICE_LOOKUP[TORCH_CPU_ID]))
    );
    entity_map->entities.push_back(
        new Entity(WIDTH/2, HEIGHT/2, 30, 1, 1, 1, 0xFF00FF00, Net({7, 64, 64, 2},DEVICE_LOOKUP[TORCH_CPU_ID]))
    );
    entity_map->entities.push_back(
        new Entity(WIDTH/2, HEIGHT/2, 30, 1, 1, 1, 0xFF0000FF, Net({7, 64, 64, 2},DEVICE_LOOKUP[TORCH_CPU_ID]))
    );
    entity_map->entities.push_back(
        new Entity(WIDTH/2, HEIGHT/2, 30, 1, 1, 1, 0xFFFF00FF, Net({7, 64, 64, 2},DEVICE_LOOKUP[TORCH_CPU_ID]))
    );
    entity_map->entities.push_back(
        new Entity(WIDTH/2, HEIGHT/2, 30, 1, 1, 1, 0xFF00FFFF, Net({7, 64, 64, 2},DEVICE_LOOKUP[TORCH_CPU_ID]))
    );
    // entity_map->entities.push_back(
    //     new Entity(WIDTH/3, HEIGHT/3, 20, 1, 1, 1, Net({7, 64, 64, 2},DEVICE_LOOKUP[TORCH_CPU_ID]))
    // );
    // entity_map->entities.push_back(
    //     new Entity(WIDTH/4, HEIGHT/4, 10, 1, 1, 1, Net({7, 64, 64, 2},DEVICE_LOOKUP[TORCH_CPU_ID]))
    // );
    // entity_map->entities.push_back(
    //     new Entity(WIDTH/5, HEIGHT/5, 5, 1, 1, 1, Net({7, 64, 64, 2},DEVICE_LOOKUP[TORCH_CPU_ID]))
    // );


    /* Event loop */
    struct timeval last_frame_time = {0, 0};
    struct timeval time1;
    struct timeval time2;
    useconds_t sleep_duration;
    while (dispatch_events(state)) {
        gettimeofday(&time1, NULL);
        
        entity_map->do_tick();
        gettimeofday(&time2, NULL);

        std::cerr << "time2 usec: " << time2.tv_usec << ", last_frame usec: " << last_frame_time.tv_usec << '\n';
        if ((time2.tv_usec - last_frame_time.tv_usec) >= (1000 * 1000 / (NUM_UNITS_PER_SEC * NUM_TICKS_PER_UNIT))
        || time2.tv_sec > last_frame_time.tv_sec) {
            request_new_frame(state);
            last_frame_time = time2;
        }

        if (time2.tv_sec > time1.tv_sec) {
            sleep_duration = 1000*1000 / (NUM_TICKS_PER_UNIT * NUM_UNITS_PER_SEC) - (UINT_MAX - time1.tv_usec + time2.tv_usec);
        }
        else {
            sleep_duration = 1000*1000 / (NUM_TICKS_PER_UNIT * NUM_UNITS_PER_SEC) - (time2.tv_usec - time1.tv_usec);
        }
        std::cerr << "Waiting to request a frame.; sleeping for " << sleep_duration << " useconds \n"; 
        usleep(sleep_duration);
    }
    return 0;
}


// torch::DeviceType device = torch::kCUDA;
// int main() {
//     Net* n = new Net({1, 1});
//     n->to(device);
//     float learning_rate = 0.01;
//     auto optimizer = torch::optim::Adam(n->parameters(), learning_rate);
//     auto loss_func = torch::nn::MSELoss();
//     int epochs = 4000;
    
//     torch::Tensor x = torch::rand({200, 1}).to(device);
//     torch::Tensor correct = torch::multiply(x, 2).to(device);
//     for (int i = 0; i < epochs; i++) {
//         optimizer.zero_grad();
//         torch::Tensor y = n->forward(x);
//         auto loss = loss_func(y, correct);
//         if (i % 10 == 0) {
//             std::cout << "###################\n";
//             std::cout << "input: \n" << x << "\n";
//             std::cout << "prediction: \n" << y << "\n";
//             std::cout << "loss: " << loss.item() << "\n";
//             std::cout << "###################\n";
//         }
//     loss.backward();
//     optimizer.step();
//     }

//     std::cout << "----- TEST SECTION -----\n";
//     for (int i = 0; i < 4; i++) {
//         std::cout << "TEST " << i << ":\n";
//         torch::Tensor testX = torch::rand({1,1}).to(device);
//         torch::Tensor testY = torch::multiply(testX, 2).to(device);
//         std::cout << "testX:\n" << testX << "\n";
//         std::cout << "testY:\n" << testY << "\n";
//         std::cout << "prediction:\n" << n->forward(testX) << "\n";
//     }
//     return 0;
// }