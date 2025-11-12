// First visit Monte carlo
#pragma once

#include "vect.hpp"
#include "RW.hpp"
#include <vector>
#include <unordered_map>

std::vector<RWActionResult> GenerateTrajectory(RWState state, vect<int> &pi, RW &rw)
{
    std::vector<RWActionResult> res;
    while (true)
    {
        RWActionResult actionRes = rw.step(state, pi.Get(state.state));
        res.push_back(actionRes);
        if (actionRes.type == GOAL || actionRes.type == PIT)
            break;
        state.state = actionRes.resState.state;
    }
    return res;
}

float FindSum(std::vector<RWActionResult> &res, float gamma)
{
    float sumResu = 0;
    for (int i = res.size() - 1; i > 0; i--)
    {
        sumResu = res[i].reward + gamma * sumResu;
    }
    sumResu += res[0].reward;
    return sumResu;
}

vect<float> MonteCarlo(vect<float> &prob, vect<int> pi, int nEpsd, float gamma, float alpha)
{
    RW rw(prob);
    vect<float> value(1, prob.row, 0);
    for (int i = 0; i < nEpsd; i++)
    {
        RWState state = RWState(2);
        std::vector<RWActionResult> trajectory = GenerateTrajectory(state, pi, rw);
        float totalReward = FindSum(trajectory, gamma);
        std::unordered_map<int, bool> map;
        for (int i = 0; i < trajectory.size(); i++)
        {
            if (map.find(state.state) != map.end())
            {
                state.state = trajectory[i].resState.state;
                totalReward = (totalReward - trajectory[i].reward) / gamma;
                continue;
            }
            value.push(state.state, value.Get(state.state) + alpha * (totalReward - value.Get(state.state)));
            // std::cout << value.Get(state.state) << std::endl;
            map[state.state] = true;
            state.state = trajectory[i].resState.state;
            totalReward = (totalReward - trajectory[i].reward) / gamma;
        }
    }
    return value;
}