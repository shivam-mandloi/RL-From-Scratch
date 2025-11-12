// Bandit Slippery Walk
#pragma once
#include <random>

#include "vect.hpp"

class BSW
{
std::random_device rd;
vect<float> rewardProb;
public:
    int numberOfbandit;
    BSW(vect<float> _rewardProb) : numberOfbandit(_rewardProb.size().Get(1)), rewardProb(_rewardProb){}

    float step(int action)
    {
        // Return reward
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        if(action >= numberOfbandit)
            exit(0);
        double randomNumber = distr(gen);
        if(randomNumber <= rewardProb.Get(action))
            return 1;
        return 0;
    }
};