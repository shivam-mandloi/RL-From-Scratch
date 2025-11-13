#pragma once

#include "vecX.hpp"
#include "SoftMax.hpp"
#include "MSE.hpp"
#include "Relu.hpp"
#include "Linear.hpp"

// 4 layer nwtwork
struct PlayerNN
{
    PlayerNN()
    : l1(4, 10), l2(10, 40), l3(40, 20), l4(20, 8)
    {
        l1.opt.SetRMSprop(0.99, 1e-3);
        l2.opt.SetRMSprop(0.99, 1e-3);
        l3.opt.SetRMSprop(0.99, 1e-3);
        l4.opt.SetRMSprop(0.99, 1e-3);
    }

    // Used only for predict
    vecX<double> PredictValueFunction(vecX<double> state)
    {
        l1.forward(state);
        rl1.forward(state);
        l2.forward(state);
        rl2.forward(state);
        l3.forward(state);
        rl3.forward(state);
        l4.forward(state);

        return state;
    }

    // Take actual value in batch
    // When learning do forward and backward propagation
    void Learn(vecX<double> states, vecX<double> actualValues)
    {
        states = PredictValueFunction(states);

        // reset the grad
        // PredictValueFunction could be used before it
        l1.ZeroGrad();l2.ZeroGrad();l3.ZeroGrad();l4.ZeroGrad();

        // Find error
        vecX<double> loss = msError.forward(states, actualValues);
        
        // Backpropagate
        vecX<double> grad = msError.backward();
        l4.backward(grad);
        rl3.backward(grad);
        l3.backward(grad);
        rl2.backward(grad);
        l2.backward(grad);
        rl1.backward(grad);
        l1.backward(grad);

        // Update weight
        l1.update();l2.update();l3.update();l4.update();
    }
private:
    Linear l1, l2, l3, l4;
    Relu rl1, rl2, rl3;
    MSE msError;
};

class MonteCarlo
{
PlayerNN player;
int randCount = 0;
float epsD;
public:
    MonteCarlo(float _epsD = 0.1) : epsD(_epsD){}

    int SelectAction(vecX<double> &predictValue)
    {
        // Select Action based on epsilon greedy method        
        std::time_t currentTime = std::time(nullptr);
        unsigned int uniqueNumber = static_cast<unsigned int>(currentTime) + randCount;
        randCount++;
        std::mt19937 gen(uniqueNumber);
        std::normal_distribution<> gaussian(0, 1);
        double epsP = gaussian(gen);
        if(epsP <= epsD)
        {
            std::uniform_int_distribution<> uniformInt(0, 7);            
            return uniformInt(gen);
        }
        int index = 0;
        for(int i = 0; i < predictValue.len; i++)
        {
            if(predictValue.Get(index) > predictValue.Get(i))
                index = i;
        }
        return index;
    }

    // main function
    int GetNextAction(vecX<double> &state)
    {
        /*
            Main Function take state as input
            state: dim(4)
                -> first two input element is position of player 1
                -> last two input element is position of player 2
            Return ACtion (0 - 7)
                -> 0: up
                -> 1: down
                -> 2: left
                -> 3: right
                -> 4: top left
                -> 5: top right
                -> 6: bottom left
                -> 7: bottom right
        */
       vecX<double> predictValue = player.PredictValueFunction(state);
       return SelectAction(predictValue);
    }

    // Take input and actual
    void Learn(vecX<double> &input, vecX<double> &actual)
    {
        player.Learn(input, actual);
    }
};