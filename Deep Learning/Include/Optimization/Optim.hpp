#pragma once

#include "SGD.hpp"
#include "RMSProp.hpp"
#include "Adam.hpp"

enum OptimType
{
    SGD_O,
    ADAM_O,
    RMSPROP_O
};

class Optim
{
    OptimType type;
    RMSprop *rMSprop;
    SGD *sgd;
    Adam *adm;

public:
    Optim()
    {
    }

    void SetRMSprop(double gamma, double lr)
    {
        rMSprop = new RMSprop(gamma, lr);
        type = RMSPROP_O;
    }

    void SetSGD(double lr)
    {
        sgd = new SGD(lr);
        type = SGD_O;
    }

    void SetAdam(double _beta1, double _beta2, double lr)
    {
        adm = new Adam(_beta1, _beta2, lr);
        type = ADAM_O;
    }

    void update(vecX<double> &weight, vecX<double> &weightGrad, vecX<double> &bias, vecX<double> &biasGrad)
    {
        if (type == SGD_O)
        {
            sgd->update(weight, weightGrad, bias, biasGrad);
        }

        if (type == RMSPROP_O)
        {
            rMSprop->update(weight, weightGrad, bias, biasGrad);
        }

        if (type == ADAM_O)
        {
            adm->update(weight, weightGrad, bias, biasGrad);
        }
    }
};