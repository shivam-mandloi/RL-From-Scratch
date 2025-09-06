#pragma once

#include "vect.hpp"
#include "RW.hpp"


vect<float> TDMethod(vect<float> &prob, vect<int> &pi, float gamma, int nEps, float alpha)
{
    vect<float> value(1, prob.row, 0);
    RW rw(prob);
    for(int i = 0; i < nEps; i++)
    {
        RWState state(2);
        while(true)
        {
            RWActionResult res = rw.step(state, pi.Get(state.state));
            value.push(state.state, value.Get(state.state) + alpha * (res.reward + gamma * value.Get(res.resState.state) - value.Get(state.state)));
            if(res.type == GOAL || res.type == PIT)
                break;
            state.state = res.resState.state;
        }
    }
    return value;
}