#pragma once

#include <vector>
#include <iostream>
#include <cmath>

#include "GridWorld.hpp"

float MaxVAlueChange(std::vector<std::vector<float>> &temp, std::vector<std::vector<float>> &value)
{
    float max = 0;
    for(int i = 0; i < temp.size(); i++)
    {
        for(int j = 0; j < temp[i].size(); j ++)
        {
            float diff = std::abs(temp[i][j] - value[i][j]);
            if(diff > max)
                max = diff;
        }
    }
    return max;
}

std::vector<std::vector<float>> PolicyEvaluation(std::vector<std::vector<int>> policy, std::vector<int> pit, float gamma, float theta)
{
    /*
        Input:
            Policy: vector with value [0,1,2,3]
            pit: vector define the pit location on grid world
            Gamma: distance penalty
            theta: Avoidable error
    */
    std::vector<std::vector<float>> value(5, std::vector<float>(5, 0));
    GridWorld world(pit);
    while(true)
    {
        std::vector<std::vector<float>> temp(value);
        for(int i = 0; i < policy.size(); i++)
        {
            for(int j = 0; j < policy[i].size(); j++)
            {
                if(i == 4 && j == 4)
                    break;
                ActionResult result = world.GetReward(State(i, j), policy[i][j]);
                if(result.type != INVALID && result.type != GOAL)
                    temp[i][j] = result.reward + gamma * temp[result.resState.x][result.resState.y];
                else
                    temp[i][j] = result.reward;
            }
        }
        if(MaxVAlueChange(temp, value) < theta)
            break;
        value = temp;
    }
    return value;
}