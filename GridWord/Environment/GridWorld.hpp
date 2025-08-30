#pragma once
#include <iostream>
#include <vector>

struct State
{
    int x, y;
    State(int _x, int _y) : x(_x), y(_y) {}
    State() : x(0), y(0) {}
    State(int a)
    {
        a -= 1;
        x = a / 5;
        y = a % 5;        
    }
    State operator-(State state1)
    {
        return State(x - state1.x, y - state1.y);
    }
};

struct ActionResult
{
    float reward;
    State resState;
    StateType type;
    ActionResult(float _reward, State _resState, StateType _type): reward(_reward), resState(_resState), type(_type){}
};

enum StateType
{
    NORMAl,
    INVALID,
    PIT,
    GOAL
};

State FindDisplacement(int a)
{
    if (a == 1)
        return State(0, 1);
    if(a == 2)
        return State(0, -1);
    if(a == 3)
        return State(1, 0);
    return State(-1, 0);
}

class GridWorld
{
std::vector<std::vector<int>> map;
public:
    GridWorld(std::vector<int> pit)
    {
        map = std::vector<std::vector<int>>(5, std::vector<int>(5, 0));
        map[4][4] = 1;
        for(int i = 0; i < pit.size(); i++)
        {
            State st(pit[i]);
            map[st.x][st.y] = 2;
        }
    }
    StateType CheckState(State state)
    {
        if(state.x < 0 || state.y < 0 || state.x > 4 || state.y > 4)
            return INVALID;
        if(map[state.x][state.y] == 2)
            return PIT;
        if(map[state.x][state.y] == 1)
            return GOAL;
        return NORMAl;
    }

    void Show(State player = State())
    {
        std::cout << "==============" << std::endl;
        for (int i = 0; i < map.size(); i++)
        {
            for (int j = 0; j < map[i].size(); j++)
            {
                if (i == player.x && j == player.y)
                    std::cout << " A ";
                else if (!map[i][j])
                    std::cout << " - ";
                else if (map[i][j] == 1)
                    std::cout << " * ";
                else if (map[i][j] == 2)
                    std::cout << " ! ";
            }
            std::cout << std::endl;
        }
        std::cout << "==============" << std::endl;
    }

    ActionResult GetReward(State player, int action)
    {
        State newPlayer = player - FindDisplacement(action);
        StateType type = CheckState(newPlayer);
        if(type == NORMAl)
            return ActionResult(0, newPlayer, NORMAl);
        if(type == GOAL)
            return ActionResult(1, newPlayer, GOAL);
        if(type == PIT)
            return ActionResult(-1, newPlayer, PIT);
        return ActionResult(-0.5, newPlayer, INVALID);
    }
};