#pragma once


#include "vect.hpp"
#include <vector>
#include "RW.hpp"

int SelectQLearningAction(std::vector<vect<float>> &Q, float epsilon, RWState state)
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

int SelectMaxAxtion(vect<float> &actions)
{
    int index = 0;
    for(int i = 0; i < actions.len; i++)
    {
        if(actions.Get(i) > actions.Get(index))
            index = i;
    }
    return index;
}

std::vector<vect<float>> QLearning(vect<float> &prob, float eps, float gamma, float alpha, int nEpsd, RWState initialState)
{
    std::vector<vect<float>> Q(prob.row, vect<float>(1, prob.col, 0));
    RW rw(prob);
    for(int i = 0; i < nEpsd; i++)
    {
        RWState state(initialState.state);
        while(true)
        {
            int action = SelectQLearningAction(Q, eps, state);
            RWActionResult res =  rw.step(state, action);
            if(res.type == GOAL || res.type == PIT)
            {
                Q[state.state].push(action, Q[state.state].Get(action) + alpha * (res.reward - Q[state.state].Get(action)));
                break;
            }
            else
            {
                int nextBestAction = SelectMaxAxtion(Q[res.resState.state]);
                Q[state.state].push(action, Q[state.state].Get(action) + alpha * (res.reward + gamma * Q[res.resState.state].Get(nextBestAction) - Q[state.state].Get(action)));
            }
            state.state = res.resState.state;
        }
    }
    return Q;
}