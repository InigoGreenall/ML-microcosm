#ifndef MODEL_HPP
#define MODEL_HPP
#include <torch/torch.h>
#include <vector>

class Net : public torch::nn::Module {
    public:
    torch::DeviceType device;
    std::vector<torch::nn::Linear> layers;
    torch::Tensor forward(torch::Tensor x);
    
    Net();
    Net(std::vector<int> layer_depths, torch::DeviceType device);

    std::vector<float> make_prediction(std::vector<float> input);
};
#endif // MODEL_HPP