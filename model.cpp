#include "model.hpp"
#include <vector>

Net::Net() : 
    Net({15, 64, 64, 2}, torch::kCUDA)
{}

Net::Net(std::vector<int> layer_depths, torch::DeviceType device):
    device(device) {
    for (int i = 0; i < layer_depths.size()-1; i++) {
        layers.push_back(register_module(
            "layer"+std::to_string(i),
            torch::nn::Linear(layer_depths[i], layer_depths[i+1])
        ));
    }
    this->to(device);
}

torch::Tensor Net::forward(torch::Tensor x) {
    for (int i = 0; i < layers.size()-1; i++) {
        x = torch::relu(layers[i]->forward(x));
    }
    x = layers[layers.size()-1]->forward(x);
    return x;
}

std::vector<float> Net::make_prediction(std::vector<float> input) {
    torch::Tensor t_input = torch::tensor(input).clone().to(device);
    torch::Tensor t_output = forward(t_input);
    return std::vector<float>(
        {t_output.index({0}).item<float>(), t_output.index({1}).item<float>()}
    );
}
