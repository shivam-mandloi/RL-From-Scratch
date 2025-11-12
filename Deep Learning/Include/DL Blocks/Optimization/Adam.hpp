#pragma once

#include "LinAlg.hpp"

class Adam
{
public:
    Adam(double _beta1, double _beta2, double _lr) : beta1(_beta1), beta2(_beta2), lr(_lr), mTB(), mTW(), vTB(), vTW(), t(1), epsilon(1e-9) {}

    void update(vecX<double> &weight, vecX<double> &weightGrad, vecX<double> &bias, vecX<double> &biasGrad)
    {
        if (mTB.row == 0)
        {
            // initialize the first and second moment
            vecX<double> tempW(weight.row, weight.col, 0), tempB(bias.row, bias.col, 0);
            mTB = tempB;
            vTB = tempB;
            mTW = tempW;
            vTW = tempW;
        }

        // mt = β_1* mt−1 + (1 − β1)*gt
        // Bias
        mTB = MatScalarProd(mTB, beta1);
        vecX<double> biasGradbeta1 = MatScalarProd(biasGrad, (1 - beta1));
        mTB = MatAdd(mTB, biasGradbeta1);

        // Weight
        mTW = MatScalarProd(mTW, beta1);
        biasGradbeta1 = MatScalarProd(weightGrad, (1 - beta1)); // use biasGradbeta1 varaible again
        mTW = MatAdd(mTW, biasGradbeta1);

        // vt = β_2 v_{t−1} + (1 − β_2)(g_t ⊙ g_t)
        // Bias
        vTB = MatScalarProd(vTB, beta2);
        vecX<double> sqrGradBias = HadamardProduct(biasGrad, biasGrad);
        biasGradbeta1 = MatScalarProd(sqrGradBias, (1 - beta2));
        vTB = MatAdd(vTB, biasGradbeta1);

        // Weight
        vTW = MatScalarProd(vTW, beta2);
        sqrGradBias = HadamardProduct(weightGrad, weightGrad); // reuse the sqrGradBias
        biasGradbeta1 = MatScalarProd(sqrGradBias, (1 - beta2));
        vTW = MatAdd(vTW, biasGradbeta1);
        // m_t = m_t / (1 − β_{1}^t)
        // Bias
        vecX<double> mBarTB = ApplyFunction(mTB, [this](double a)
                                            { return a / (1 - std::pow(beta1, t)); });
        vecX<double> vBarTB = ApplyFunction(vTB, [this](double a)
                                            { return a / (1 - std::pow(beta2, t)); });
        // weight
        vecX<double> mBarTW = ApplyFunction(mTW, [this](double a)
                                            { return a / (1 - std::pow(beta1, t)); });
        vecX<double> vBarTW = ApplyFunction(vTW, [this](double a)
                                            { return a / (1 - std::pow(beta2, t)); });

        // w_{t} = w_{t−1} − (η / sqrt(v_t + ϵ)) m_t
        // Bias
        vBarTB = ApplyFunction(vBarTB, [this](double a)
                               { return -lr / std::sqrt(a + epsilon); });
        vBarTB = HadamardProduct(vBarTB, mBarTB);
        // vBarTB.print();
        bias = MatAdd(bias, vBarTB);

        // Weight
        vBarTW = ApplyFunction(vBarTW, [this](double a)
                               { return -lr / std::sqrt(a + epsilon); });
        vBarTW = HadamardProduct(vBarTW, mBarTW);
        // vBarTW.print();
        weight = MatAdd(weight, vBarTW);
        t += 1;
        // std::cout << "why this error" << std::endl;
    }

private:
    int t;
    double lr, beta1, beta2, epsilon;
    vecX<double> mTB, vTB, mTW, vTW;
};