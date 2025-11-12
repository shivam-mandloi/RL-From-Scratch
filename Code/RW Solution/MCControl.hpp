#pragma once

#include "vect.hpp"
#include "RW.hpp"
#include <vector>
#include <unordered_map>

int SelectAction(std::vector<vect<float>> &Q, float epsilon, RWState state)
{
    // Greedy Epsilon
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distr(0.0, 1.0);
    double randomNumber = distr(gen);

    // epsilon probability to select random action
    if (randomNumber < epsilon)
    {
        std::uniform_int_distribution<int> distr(0, Q[state.state].len - 1);
        int index = distr(gen);
        return index;
    }

    int index = 0;
    for (int i = 0; i < Q[state.state].len; i++)
    {
        if (Q[state.state].Get(i) > Q[state.state].Get(index))
            index = i;
    }
    return index;
}

std::vector<RWActionResult> GenerateTrajectory(vect<float> &prob, std::vector<vect<float>> &Q, RWState state, float eps)
{
    RW rw(prob);
    std::vector<RWActionResult> res;
    int iter = 0;
    while (iter <= 20)
    {
        int action = SelectAction(Q, eps, state);
        RWActionResult result = rw.step(state, action);
        res.push_back(result);
        if (result.type == PIT || result.type == GOAL)
            break;
        state.state = result.resState.state;
        iter++;
    }
    return res;
}

float FindDiscountedSum(std::vector<RWActionResult> trajectory, float gamma)
{
    float sum = 0;
    for (int i = trajectory.size() - 1; i > -1; i--)
    {
        sum = trajectory[i].reward + gamma * sum;
    }
    return sum;
}

std::vector<vect<float>> MCControl(vect<float> prob, float eps, float gamma, float alpha, int nEpsd)
{
    // eps = probability for exploration
    // gamma = discounted reward
    // alpha = step size
    std::vector<vect<float>> Q(prob.row, vect<float>(1, prob.col, 0));
    for (int epc = 0; epc < nEpsd; epc++)
    {
        std::vector<RWActionResult> trajectory = GenerateTrajectory(prob, Q, RWState(2), eps);
        float discountedSum = FindDiscountedSum(trajectory, gamma);
        std::unordered_map<int, bool> map;
        RWState state(2);
        for (int i = 0; i < trajectory.size(); i++)
        {
            if (map.find(state.state) != map.end())
            {
                state = trajectory[i].resState.state;
                discountedSum = (discountedSum - trajectory[i].reward) / gamma;
                continue;
            }
            Q[state.state].push(trajectory[i].action, Q[state.state].Get(trajectory[i].action) + alpha * (discountedSum - Q[state.state].Get(trajectory[i].action)));            
            discountedSum = (discountedSum - trajectory[i].reward) / gamma;
            map[state.state] = true;
            state.state = trajectory[i].resState.state;
        }
        eps -= 0.001;
    }
    return Q;
}