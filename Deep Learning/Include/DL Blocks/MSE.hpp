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
        savedActual = MatScalarProd(savedActual, -1);
        savedInput = MatAdd(savedInput, savedActual);
        savedInput = MatScalarProd(savedInput, 2/(double)savedActual.row);
        savedInput.TR();
        return savedInput;
    }

private:
    vecX<double> savedInput, savedActual;
};