#pragma once

#include "LinAlg.hpp"

class CrossEntropyLoss
{
public:
    CrossEntropyLoss(){}

    vecX<double> forward(vecX<double> &pred, vecX<int> &actual)
    {
        /*
            => Assume input in column vector
            (Input) n X m =>
                n = dim of input
                m = batch size
            => return column vector
        */
        vecX<double> loss(pred.col, 1, 0);
        savedPred = pred;
        index = actual;        
        for(int i = 0; i < pred.col; i++)
        {
            loss.push(i, -std::log(pred.Get(actual.Get(i), i)));
        }
        return loss;
    }

    vecX<double> backward()
    {
        /*
           => savedPred dimension is like this
            (Input) n X m =>
                n = dim of input
                m = batch size
            
            => actual have this dim
            (Input) m => column vector
            
            => return: matrix (m X n)
        */
        vecX<double> grad(savedPred.col, savedPred.row, 0);
        for(int i = 0; i < savedPred.col; i++)
        {
            grad.push(i, index.Get(i), -1 / savedPred.Get(index.Get(i), i));
        }
        return grad;
    }

private:
    vecX<double> savedPred;
    vecX<int> index;    
};