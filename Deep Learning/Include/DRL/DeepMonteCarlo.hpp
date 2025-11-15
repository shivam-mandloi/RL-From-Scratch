#pragma once

#include "vecX.hpp"
#include "SoftMax.hpp"
#include "MSE.hpp"
#include "Relu.hpp"
#include "Linear.hpp"

// 4 layer nwtwork
struct PlayerNN
{
    // Input will be
    // 1st player location, 1st player velocity, 2nd player location, 2nd player velocity
    PlayerNN()
        : l1(9, 10), l2(10, 40), l3(40, 20), l4(20, 9)
    {
        l1.opt.SetRMSprop(0.99, 1e-4);
        l2.opt.SetRMSprop(0.99, 1e-4);
        l3.opt.SetRMSprop(0.99, 1e-4);
        l4.opt.SetRMSprop(0.99, 1e-4);

        // l1.opt.SetSGD(1e-5);
        // l2.opt.SetSGD(1e-5);
        // l3.opt.SetSGD(1e-5);
        // l4.opt.SetSGD(1e-5);
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

        return state;
    }

    // Take actual value in batch
    // When learning do forward and backward propagation
    double Learn(vecX<double> states, std::vector<int> &selectedAction, std::vector<float> &allReward, float cumReward, float gamma)
    {
        states = PredictValueFunction(states);
        // reset the grad
        // PredictValueFunction could be used before it
        l1.ZeroGrad();
        l2.ZeroGrad();
        l3.ZeroGrad();
        l4.ZeroGrad();

        vecX<double> actualValues = states;
        for (int i = 0; i < states.col; i++)
        {
            actualValues.push(selectedAction[i], i, cumReward);
            cumReward = (cumReward - allReward[i]) / gamma;
        }
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
        l4.backward(grad);
        rl3.backward(grad);
        l3.backward(grad);
        rl2.backward(grad);
        l2.backward(grad);
        rl1.backward(grad);
        l1.backward(grad);
        
        // Update weight
        l1.update();
        l2.update();
        l3.update();
        l4.update();
        return totalLoss;
    }

private:
    Linear l1, l2, l3, l4;
    Relu rl1, rl2, rl3;
    MSE msError;
};

class DeepMonteCarlo
{
    PlayerNN player;
    int randCount = 0;
    std::vector<vecX<double>> allState;
    std::vector<int> selectedAction;
    std::vector<float> allReward;

public:
    float epsD, totalReward = 0.f, gamma;
    DeepMonteCarlo(float _epsD = 0.3, float _gamma = 0.9) : epsD(_epsD), gamma(_gamma) {}

    int SelectAction(vecX<double> &predictValue)
    {
        // Select Action based on epsilon greedy method
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        double randomNumber = distr(gen);
        if (randomNumber <= epsD)
        {
            std::uniform_int_distribution<> uniformInt(0, 8);
            return uniformInt(gen);
        }
        int index = 0;
        for (int i = 0; i < predictValue.len; i++)
        {
            if (predictValue.Get(index) > predictValue.Get(i))
                index = i;
        }
        return index;
    }

    // main function
    int GetNextAction(vecX<double> state)
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
                -> 8: No Change
        */
        state.TR();
        vecX<double> predictValue = player.PredictValueFunction(state);
        return SelectAction(predictValue);
    }

    // Take input and actual
    double Learn()
    {
        vecX<double> states = ConvertVectorToVecX(allState);

        // States is batch of row vector
        states.TR();
        double loss = player.Learn(states, selectedAction, allReward, totalReward, gamma);

        // Reset the state and reward
        allState = {};
        totalReward = 0;
        return loss;
    }

    // save each state, based on selected action and result from the environment
    void SaveState(vecX<double> state, float reward, int action)
    {
        allState.push_back(state);
        totalReward = reward + gamma * totalReward;
        allReward.push_back(reward);
        selectedAction.push_back(action);
    }
};