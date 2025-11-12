#pragma once

#include "LinAlg.hpp"

class SGD
{
public:
    SGD(double _lr) : lr(_lr){}

    void update(vecX<double> &weight, vecX<double> &weightGrad, vecX<double> &bias, vecX<double> &biasGrad)
    {
        weightGrad = MatScalarProd(weightGrad, -lr);
        biasGrad = MatScalarProd(biasGrad, -lr);

        weight = MatAdd(weight, weightGrad);
        bias = MatAdd(bias, biasGrad);
    }
    
private:
    double lr;
};