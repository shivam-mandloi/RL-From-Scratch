#pragma once

#include "GridWorld.hpp"

int GetMaxAction(std::vector<std::vector<float>> &value, std::vector<int> &pit, State player)
{
    float MaxReward = -100;
    int action = -1;
    
    GridWorld world(pit);
    for(int i = 0; i < 4; i++)
    {
        ActionResult res = world.GetReward(player, i);
        float q = res.reward;
        if(res.type != INVALID && res.type != GOAL)
            q += value[res.resState.x][res.resState.y];
        if(MaxReward < q)
        {
            MaxReward = q;
            action = i;
        }
    }
    return action;
}


std::vector<std::vector<int>> PolicyImprovement(std::vector<std::vector<float>> &value, std::vector<int> &pit)
{
    std::vector<std::vector<int>> policy(5, std::vector<int>(5, 0));
    for(int i = 0; i < value.size(); i++)
    {
        for(int j = 0; j < value[i].size(); j++)
        {
            policy[i][j] = GetMaxAction(value, pit, State(i,j));
        }
    }
    return policy;
}