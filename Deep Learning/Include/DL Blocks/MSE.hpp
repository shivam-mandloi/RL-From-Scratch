#pragma once

#include "LinAlg.hpp"

class MSE
{
public:
    vecX<double> forward(vecX<double> &input, vecX<double> &actual)
    {
         /*
            => Assume input in column vector
            (Input) n X m =>
                n = dim of input
                m = batch size
            => return sinlge vector
        */
        savedActual = actual; savedInput = input;
        vecX<double> loss(1, actual.col, 0);
        for(int i = 0; i < input.col; i++)
        {
            double sumLoss = 0.0;
            for(int j = 0; j < input.row; j++)
            {
                sumLoss += (actual.Get(j, i) - input.Get(j, i)) * (actual.Get(j, i) - input.Get(j, i));
            }
            loss.push(i, sumLoss / input.row);
        }
        
        return loss;
    }

    vecX<double> backward()
    {
        /*
            (predict) n X m =>

            (Actual) n X m =>

            (Return) m X n => 
                n = dim of input
                m = batch size
         */
        // vecX<double> prevGrad(savedActual.col, savedActual.row, 0);
        savedActual = MatScalarProd(savedActual, -1);
        vecX<double> savedInput = MatAdd(savedInput, savedActual);
        MatScalarProd(savedInput, 2/savedActual.row);
        return savedInput;
    }

private:
    vecX<double> savedInput, savedActual;
};