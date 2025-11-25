#pragma once

#include "vecX.hpp"
#include "SoftMax.hpp"
#include "MSE.hpp"
#include "Relu.hpp"
#include "Linear.hpp"

#include <deque>

struct ReplayBufferNode
{
    vecX<double> state; // input to NN
    float target; // target for perticular action
    int action; // action which was selected    
    ReplayBufferNode(vecX<double> _state, float _target, int _action) : state(_state), target(_target), action(_action){}
};

// 4 layer nwtwork
struct PlayerNN
{
    std::deque<ReplayBufferNode *> replayBuffer;
    int qSize, batchSize;
    // Input will be
    // 1st player location, 1st player velocity, 2nd player location, 2nd player velocity
    PlayerNN(int _qSize = 512, int _batchSize = 128)
        : l1(9, 10), l2(10, 40), l3(40, 20), l4(20, 9), qSize(_qSize), batchSize(_batchSize)
    {
        l1.opt.SetRMSprop(0.99, 5e-5);
        l2.opt.SetRMSprop(0.99, 5e-5);
        l3.opt.SetRMSprop(0.99, 5e-5);
        l4.opt.SetRMSprop(0.99, 5e-5);

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
    double Learn()
    {        
        // reset the grad
        // PredictValueFunction could be used before it
        l1.ZeroGrad();l2.ZeroGrad();l3.ZeroGrad();l4.ZeroGrad();
        
        int batchCalc = std::min(batchSize, (int)replayBuffer.size());
        vecX<double> input(replayBuffer[0]->state.row, batchCalc, 0); // StateDim X Batch Size
        std::vector<int> shuffledVec = GetNShuffledPoint(replayBuffer.size());
        for(int i = 0; i < batchCalc; i++)
        {
            for(int j = 0; j < input.row; j++)
            {
                double ele = replayBuffer[shuffledVec[i]]->state.Get(j);
                input.push(j, i, ele);
            }
        }
        input = PredictValueFunction(input);

        // Find Target
        vecX<double> target = input; // change only target for action we take
        for(int i = 0; i < input.col; i++) // for each input in batch
        {
            target.push(replayBuffer[shuffledVec[i]]->action, i, replayBuffer[shuffledVec[i]]->target);
        }

        // Find error
        vecX<double> loss = msError.forward(input, target);
        double totalLoss = 0;

        loss.size().print();

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

    void AddInBuffer(vecX<double> &state, float target, int action)
    {
        if(replayBuffer.size() >= qSize)
            replayBuffer.pop_front();
        ReplayBufferNode *node = new ReplayBufferNode(state, target, action);
        replayBuffer.push_back(node);
    }

private:
    Linear l1, l2, l3, l4;
    Relu rl1, rl2, rl3;
    MSE msError;
};

class DeepQLearning
{
PlayerNN player;
std::vector<ReplayBufferNode *> replayBuffer;
public:
    float epsD, totalReward, gamma;
    DeepQLearning(float _epsD = 0.3, float _gamma = 0.99) : epsD(_epsD), totalReward(0), gamma(_gamma){}

    int GetMaxAction(vecX<double> &predictValue)
    {
        int index = 0;
        for (int i = 0; i < predictValue.len; i++)
        {
            if (predictValue.Get(index) > predictValue.Get(i))
                index = i;
        }
        return index;
    }

    int SelectAction(vecX<double> &predictValue)
    {
        // Select Action based on epsilon greedy method
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        double randomNumber = distr(gen);
        
        if (randomNumber <= epsD)
        {
            std::uniform_int_distribution<> uniformInt(0, 8); // 8 actions
            return uniformInt(gen);
        }
        return GetMaxAction(predictValue);
    }

    // main function
    int GetNextAction(vecX<double> state)
    {
        /*
            Main Function take state as input
            state: dim(4)
                -> first two input element is position of player 1
                -> last two input element is position of player 2
                -> It must be a column vector
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
        vecX<double> predictValue = player.PredictValueFunction(state);
        return SelectAction(predictValue);
    }

    // Take input and actual
    double Learn()
    {
        totalReward = 0;
        return player.Learn();
    }

    // save each state, based on selected action and result from the environment
    void SaveState(vecX<double> state, int action, vecX<double> nextState, float reward, bool isTerminalState = false)
    {
        /*
            state must be a column vector
        */
        // update the replay buffer
        totalReward += reward;
        if(!isTerminalState)
        {
            vecX<double> pred = player.PredictValueFunction(nextState);
            int maxAction = GetMaxAction(pred);
            reward += gamma * pred.Get(maxAction);
        }
        player.AddInBuffer(state, reward, action);
    }
};