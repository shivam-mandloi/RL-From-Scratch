#pragma once

#include "LinAlg.hpp"

#define eps 1e-8

class RMSprop
{
public:
    RMSprop(double _gamma, double _lr) : gamma(_gamma), lr(_lr), epsilon(eps), eGW(), eGB() {}

    void update(vecX<double> &weight, vecX<double> &weightGrad, vecX<double> &bias, vecX<double> &biasGrad)
    {
        if (!eGW.arr)
        {
            vecX<double> tempW(weight.row, weight.col, 0), tempB(bias.row, bias.col, 0);
            eGW = tempW;
            eGB = tempB;
        }

        // eGB = gamma * eGB + (1 - gamma) * g^2
        eGB = MatScalarProd(eGB, gamma); // gamma * eGB
        vecX<double> biasGradSq = HadamardProduct(biasGrad, biasGrad);

        biasGradSq = MatScalarProd(biasGradSq, (1 - gamma)); // (1 - gamma) * g^2
        eGB = MatAdd(eGB, biasGradSq);

        // eGW = gamma * eGW + (1 - gamma) * g^2
        eGW = MatScalarProd(eGW, gamma);                                     // gamma * eGW
        vecX<double> weightGradSq = HadamardProduct(weightGrad, weightGrad); // (1 - gamma) * g^2
        weightGradSq = MatScalarProd(weightGradSq, (1 - gamma));
        eGW = MatAdd(eGW, weightGradSq);
        vecX<double> erGB = ApplyFunction(eGB, [this](double a)
                                          { return -lr / (std::pow(a + eps, 0.5)); }); // n / (sqrt(eGW + eps)^0.5)
        vecX<double> erGW = ApplyFunction(eGW, [this](double a)
                                          { return -lr / (std::pow(a + eps, 0.5)); }); // n / (sqrt(eGB + eps)^0.5)

        weightGrad = HadamardProduct(weightGrad, erGW);
        biasGrad = HadamardProduct(biasGrad, erGB);

        weight = MatAdd(weight, weightGrad);
        bias = MatAdd(bias, biasGrad);
    }

private:
    double lr, gamma, epsilon;
    vecX<double> eGW, eGB;
};