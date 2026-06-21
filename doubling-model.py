#!/bin/python3
import torch
import torch.nn as nn
import torch.optim as optim

class Model(nn.Module):
    def __init__(self, in_features=1, l1=4, out_features=1):
        super().__init__()
        self.activ_func = nn.ReLU()
        self.to_layer1 = nn.Linear(in_features, l1)
        self.to_out = nn.Linear(l1, out_features)

    def forward(self, x):
        # x = self.activ_func(self.to_layer1(x))
        x = self.to_layer1(x)
        x = self.to_out(x)
        return x


dev = torch.device("cpu")

model = Model(in_features=1, out_features=1).to(device=dev)
learning_rate = 0.01
optimizer = optim.Adam(params=model.parameters(), lr=learning_rate)
loss_func = nn.MSELoss()
epochs = 4000

X = torch.rand(200, 1).to(device=dev)
correct = torch.multiply(X, 2).to(device=dev)
print(X)
print(correct)
# exit()
for epoch in range(epochs):
    optimizer.zero_grad()
    Y = model.forward(X)
    loss = loss_func(Y, correct)
    if epoch % 10 == 0:
        print("###################")
        print(f"input: {X}")
        print(f"prediction: {Y}")
        print(f"loss: {loss.item()}")
        print("###################")
    loss.backward()
    optimizer.step()

print("----- TEST SECTION -----")
for i in range(4):
    print(f"TEST {i}:")
    testX = torch.randn(1,1).to(device=dev)
    testY = torch.multiply(testX, 2).to(device=dev)
    print(f"testX: {testX}")
    print(f"testY: {testY}")
    print(f"prediction: {model.forward(testX)}")