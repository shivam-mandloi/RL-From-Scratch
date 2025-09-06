// Random Walk
#pragma once
#include <vect.hpp>
#include <random>

struct RWState
{
    int state;
    RWState(int _state) : state(_state){}
    RWState() : state(0){}
};

enum RWStateType
{
    NORMAl,    
    PIT,
    GOAL
};

struct RWActionResult
{
    float reward;
    RWState resState;
    RWStateType type;
    RWActionResult(float _reward, RWState _resState, RWStateType _type): reward(_reward), resState(_resState), type(_type){}
    RWActionResult(const RWActionResult &other) : reward(other.reward), resState(other.resState), type(other.type){}
};

class RW
{
int nonTerminalStates;
vect<float> prob;
std::random_device rd;
public:
    // prob = nonTerminalStates X 2
    RW(vect<float> _prob) : nonTerminalStates(_prob.row), prob(_prob) {}

    // action = 0 -> left | action = 1 -> right
    RWActionResult step(RWState state, int action)
    {
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        double randomNumber = distr(gen);

        if(randomNumber > prob.Get(state.state, action))
            action = 1 - action;
        state.state += (action == 1) ? 1 : -1;

        if(state.state == -1)
            return RWActionResult(-1, state.state, PIT);
        if(state.state == prob.row)
            return RWActionResult(1, state.state, GOAL);
        return RWActionResult(0, state.state, NORMAl);
    }
};