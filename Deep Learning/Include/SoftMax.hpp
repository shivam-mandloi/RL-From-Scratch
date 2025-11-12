#pragma once

#include "LinAlg.hpp"
#include "GradHelpingFunc.hpp"
#include <functional>

/*
    forward -> Input <column vector> / return <column vector>

    backward -> Input <column vector> / return <column vector>
*/

std::function<double(double, double, double)> clip = [](double minVal, double maxVal, double val){
    return val > maxVal ? maxVal : (minVal > val ? minVal : val);
};

class Softmax
{
public:
    Softmax(double _temprature = 1): temprature(_temprature){}
    void forward(vecX<double> &input)
    {
        /*
            => Assume input in column vector
            (Input) n X m =>
                n = dim of input
                m = batch size
            => return matrix of same size
        */
        vecX<double> total(input.col, 1, 0);
        for(int i = 0; i < input.col; i++)
        {
            double totalSum = 0.0;
            for(int j = 0; j < input.row; j++)
            {
                double temp = std::exp(clip(-50, 50, input.Get(j, i)/temprature));
                input.push(j, i, temp);
                totalSum += temp;
            }
            total.push(i, 0, totalSum);
        }
        for(int i = 0; i < input.row; i++)
        {
            for(int j = 0; j < input.col; j++)
            {
                input.push(i, j, input.Get(i, j)/total.Get(j,0));
            }
        }
        prob = input;
    }

    void backward(vecX<double> &prevGrad)
    {
        // we assume that prevGrad is row vector (batch size X input dim)
        SoftMaxGrad(prevGrad, prob); // update preGrad
    }

private:
    double temprature;
    vecX<double> prob;
};