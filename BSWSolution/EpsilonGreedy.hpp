#pragma once

#include "BSW.hpp"

// select random action
int RandomAction(vect<float> &value)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distr(0.0, value.size().Get(1));
    return int(distr(gen));
}

// select action which has max value
int MaxAction(vect<float> &value)
{
    int index = 0;
    for(int i = 0; i < value.len; i++)
    {
        if(value.Get(index) > value.Get(i))
            index = i;
    }
    return index;
}

// epsilon greedy method to select action
int SelectAction(float eps, vect<float> &value)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distr(0.0, value.size().Get(1));
    float isExplr = int(distr(gen));
    if(isExplr < eps)
        return RandomAction(value);
    return MaxAction(value);
}

vect<float> EpsilonGreedy(int noEps, vect<float> &prob, float eps)
{
    BSW env(prob);
    vect<float> value(1, prob.len, 0);
    vect<int> timeIter(1, prob.len, 0);
    for(int i = 0; i < noEps; i++)
    {
        int action = SelectAction(eps, value);
        float reward = env.step(action);
        timeIter.push(action, timeIter.Get(action) + 1);
        value.push(action, value.Get(action) + (reward - value.Get(action))/timeIter.Get(action));
    }
    return value;
}