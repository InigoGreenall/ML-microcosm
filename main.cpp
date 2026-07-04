#include "main.hpp"
#include "dtypes.hpp"
#include "model.hpp"
#include "softwarerender.hpp"
#include <unistd.h>

extern "C" {
    #include "testing-include/wlclient.h"
}

static const torch::DeviceType DEVICE_LOOKUP[] = {torch::kCPU, torch::kCUDA};

EntityMap* entity_map;

int main() {
    struct state_t* state = init(WIDTH, HEIGHT);
    install_frame_drawer(state, frame_drawer);

    // TODO: do program setup
    entity_map = new EntityMap;
    entity_map->entities.push_back(
        new Entity(WIDTH/2, HEIGHT/2, 50, 1, 1, 1, Net({7, 64, 64, 2},DEVICE_LOOKUP[TORCH_CPU_ID]))
    );
        entity_map->entities.push_back(
        new Entity(WIDTH/3, HEIGHT/3, 35, 1, 1, 1, Net({7, 64, 64, 2},DEVICE_LOOKUP[TORCH_CPU_ID]))
    );


    while (dispatch_events(state)) {

        // TODO: event loop
        entity_map->do_tick();



        request_new_frame(state);
        usleep(1000*1000 / 8);
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