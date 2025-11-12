#pragma once

#include "LinAlg.hpp"

/*
    forward -> Input <column vector> / return <column vector>

    backward -> Input <column vector> / return <column vector>
*/

class Relu
{
public:
    void forward(vecX<double> &input)
    {
        saved = input;
        for(int i = 0; i < input.len; i++)
        {
            if(input.Get(i) < 0)
                input.push(i, 0);
        }
    }

    void backward(vecX<double> &prevGrad)
    {
        // make every element to zero, correspond to the input
        for(int i = 0; i < prevGrad.len; i++)
        {
            if(saved.Get(i) < 0)
                prevGrad.push(i, 0);
        }
    }
private:
    vecX<double> saved;
};