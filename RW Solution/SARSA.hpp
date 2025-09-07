#pragma once

#include "vect.hpp"
#include "RW.hpp"
#include <vector>

int SelectSARSAAction(std::vector<vect<float>> &Q, float epsilon, RWState state)
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

std::vector<vect<float>> SARSA(vect<float> prob, float eps, float gamma, float alpha, int nEpsd, RWState initialState)
{
    RW rw(prob);
    std::vector<vect<float>> Q(prob.row, vect<float>(1, prob.col, 0));
    for(int i = 0; i < nEpsd; i++)
    {
        RWState state(initialState.state);
        int action = SelectSARSAAction(Q, eps, state);
        while(true)
        {
            RWActionResult res = rw.step(state, action);
            float curStateQ = Q[state.state].Get(action);
            
            if(res.type == GOAL || res.type == PIT)
            {
                Q[state.state].push(action, Q[state.state].Get(action) + alpha * (res.reward - curStateQ));
                break;
            }
            
            int nextAction = SelectSARSAAction(Q, eps, res.resState);            
            float nextStateQ = Q[res.resState.state].Get(nextAction);
            Q[state.state].push(action, Q[state.state].Get(action) + alpha * (res.reward + gamma * nextStateQ - curStateQ));

            state.state = res.resState.state;
            action = nextAction;
        }
    }
    return Q;
}