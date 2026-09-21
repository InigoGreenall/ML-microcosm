#include "entity/entity_map.hpp"
#include "graphics/renderer.hpp"
#include <chrono>
#include <thread>

constexpr int TORCH_CPU_ID = 0;
constexpr int TORCH_CUDA_ID = 1;
static const torch::DeviceType DEVICE_LOOKUP[] = {torch::kCPU, torch::kCUDA};

EntityMap* entity_map;

int main() {
    state_t* state = init(WIDTH, HEIGHT);

    // TODO: do program setup
    entity_map = new EntityMap;
    entity_map->entities.push_back(
        new Entity(WIDTH/4, HEIGHT/4, 10, 1, 1, 1, 0xFFFF0000, Net({7, 64, 64, 2},DEVICE_LOOKUP[TORCH_CPU_ID]))
    );
    entity_map->entities.push_back(
        new Entity(WIDTH/4, HEIGHT/4, 10, 1, 1, 1, 0xFFFF0000, Net({7, 64, 64, 2},DEVICE_LOOKUP[TORCH_CPU_ID]))
    );
    entity_map->entities.push_back(
        new Entity(WIDTH/4, HEIGHT/4, 10, 1, 1, 1, 0xFFFF0000, Net({7, 64, 64, 2},DEVICE_LOOKUP[TORCH_CPU_ID]))
    );
    entity_map->entities.push_back(
        new Entity(WIDTH/4, HEIGHT/4, 10, 1, 1, 1, 0xFFFF0000, Net({7, 64, 64, 2},DEVICE_LOOKUP[TORCH_CPU_ID]))
    );
    entity_map->entities.push_back(
        new Entity(WIDTH/4, HEIGHT/4, 10, 1, 1, 1, 0xFFFF0000, Net({7, 64, 64, 2},DEVICE_LOOKUP[TORCH_CPU_ID]))
    );
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
    std::chrono::steady_clock clock;
    auto last_frame_time = clock.now();
    while (dispatch_events(state) != false) {
        auto time1 = clock.now();
        entity_map->do_tick();
        auto time2 = clock.now();

        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time2 - last_frame_time);
        if ((elapsed.count() >= (1000*1000 / (NUM_UNITS_PER_SEC * NUM_TICKS_PER_UNIT)))) {
            request_new_frame(state);
            last_frame_time = time2;
        }

        auto sleep_duration = std::chrono::microseconds(1000*1000 / (NUM_TICKS_PER_UNIT * NUM_UNITS_PER_SEC)) 
            - std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1);
        
        std::cerr << "Waiting to request a frame; sleeping for " << sleep_duration.count() << " microseconds \n"; 
        std:std::this_thread::sleep_for(sleep_duration);
    }
    return 1;
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