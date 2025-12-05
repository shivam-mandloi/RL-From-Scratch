#pragma once

#include "vecX.hpp"
#include "SoftMax.hpp"
#include "MSE.hpp"
#include "Relu.hpp"
#include "Linear.hpp"

// Used by all the DRL methods
// 6 layer nwtwork
struct NetworkValue
{
    NetworkValue()
        : l1(5, 10), l2(10, 40), l3(40, 30), l4(30, 20), l5(20, 9)
    {
        l1.opt.SetRMSprop(0.99, 1e-4);
        l2.opt.SetRMSprop(0.99, 1e-4);
        l3.opt.SetRMSprop(0.99, 1e-4);
        l4.opt.SetRMSprop(0.99, 1e-4);
        l5.opt.SetRMSprop(0.99, 1e-4);
    }

    // Used only for predict
    vecX<double> PredictValueFunction(vecX<double> &state)
    {
        l1.forward(state);
        rl1.forward(state);
        l2.forward(state);
        rl2.forward(state);
        l3.forward(state);
        rl3.forward(state);
        l4.forward(state);
        rl4.forward(state);
        l5.forward(state);
        rl5.forward(state); // output always positive
        return state;
    }

    // Take actual value in batch
    // When learning do forward and backward propagation
    double Learn(vecX<double> states, vecX<double> actualValues)
    {
        states = PredictValueFunction(states);
        // reset the grad
        // PredictValueFunction could be used before it
        l1.ZeroGrad();
        l2.ZeroGrad();
        l3.ZeroGrad();
        l4.ZeroGrad();
        l5.ZeroGrad();

        // Find error
        vecX<double> loss = msError.forward(states, actualValues);
        double totalLoss = 0;

        // Calculate the mean loss
        for(int i = 0; i < loss.len; i++)
        {
            totalLoss += loss.Get(i);
        }
        totalLoss /= loss.len;

        // Backpropagate
        vecX<double> grad = msError.backward();
        rl5.backward(grad);
        l5.backward(grad);
        rl4.backward(grad);
        l4.backward(grad);
        rl3.backward(grad);
        l3.backward(grad);
        rl2.backward(grad);
        l2.backward(grad);
        rl1.backward(grad);
        l1.backward(grad);
        
        // Update weights
        l1.update();
        l2.update();
        l3.update();
        l4.update();
        l5.update();        
        return totalLoss;
    }

private:
    Linear l1, l2, l3, l4, l5;
    Relu rl1, rl2, rl3, rl4, rl5;
    MSE msError;
};