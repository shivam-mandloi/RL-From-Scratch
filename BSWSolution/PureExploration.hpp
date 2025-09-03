#pragma once

#include "vect.hpp"
#include "BSW.hpp"

int GetRandomAction(vect<float> &value)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distr(0.0, value.size().Get(1));
    return int(distr(gen));
}

vect<float> PureExploration(int noEps, vect<float> prob)
{
    BSW env(prob);
    vect<float> value(1, env.numberOfbandit, 0.0);
    vect<int> freqOfAction(1, env.numberOfbandit, 0);
    for(int i = 0; i < noEps; i++)
    {
        int action = GetRandomAction(value);
        float reward = env.step(action);
        freqOfAction.push(action, freqOfAction.Get(action) + 1);
        value.push(action, value.Get(action) + (reward - value.Get(action)) / freqOfAction.Get(action));
    }
    return value;
}