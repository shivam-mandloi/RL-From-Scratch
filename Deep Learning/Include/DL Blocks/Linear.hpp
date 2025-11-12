#pragma once

#include "LinAlg.hpp"
#include "Optim.hpp"
#include "GradHelpingFunc.hpp"

/*
    forward -> Input <column vector> / return <column vector>

    backward -> Input <column vector> / return <column vector>
*/

class Linear
{
public:
    Optim opt;
    Linear(int inFeature, int outFeature, double lr = 0.01) : weigthUpdate(outFeature, inFeature, 0), biasUpdate(outFeature, 1, 0)
    {
        weight = RandomVecX(outFeature, inFeature, 0.0, 0.1);
        bias = RandomVecX(outFeature, 1, 0.0, 0.1);
    }

    void forward(vecX<double> &input)
    {
        saved = input;
        input = MatMul(weight, input); // wx
        // Mat Add take care about the different size of matrices
        input = MatAdd(input, bias); // wx + b
    }

    void backward(vecX<double> &prevGrad)
    {
        // std::cout << "linear grad start" << std::endl;
        LinearBackward(prevGrad, weight, bias, saved, weigthUpdate, biasUpdate);
    }

    void update()
    {
        // update the weights
        opt.update(weight, weigthUpdate, bias, biasUpdate);

        // change update vector to zero
        std::fill(weigthUpdate.arr, weigthUpdate.arr + weigthUpdate.len, 0.0);
        std::fill(biasUpdate.arr, biasUpdate.arr + biasUpdate.len, 0.0);
    }

private:
    vecX<double> weight, bias, saved, weigthUpdate, biasUpdate;
};