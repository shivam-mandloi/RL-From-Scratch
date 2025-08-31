#pragma once

#include "GridWorld.hpp"
#include "PolicyEvaluation.hpp"
#include "PolicyImprovement.hpp"

bool IsSame(std::vector<std::vector<int>> policy, std::vector<std::vector<int>> temp)
{
    for(int i = 0; i < policy.size(); i++)
    {
        for(int j = 0; j < policy[i].size(); j++)
        {
            if(policy[i][j] != temp[i][j])
                return false;
        }
    }
    return true;
}


std::vector<std::vector<int>> PolicyIteration(std::vector<int> &pit, float gamma, float theta)
{
    std::vector<std::vector<int>> policy(5, std::vector<int>(5, 0));
    std::vector<std::vector<float>> value(5, std::vector<float>(5, 0));

    while(true)
    {
        value = PolicyEvaluation(policy, pit, gamma, theta);
        std::vector<std::vector<int>> temp = PolicyImprovement(value, pit);
        if(IsSame(policy, temp))
            break;
        policy = temp;
    }
    return policy;
}