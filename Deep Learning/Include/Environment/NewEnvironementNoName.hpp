#pragma once

#include "IncludeSFML.hpp"
#include "vecX.hpp"
#include <iostream>
#include <random>
#include <algorithm>

int GiveRandomInt(int start, int last)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> uniformInt(start, last);
    return uniformInt(gen);
}

float GiveRandomFloat(float a, float b)
{
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distr(a, b);
    return distr(gen);
}

class Player
{
int borderX, borderY;
public:
    std::vector<float> position, velocity;
    sf::CircleShape player;
    float radius;
    std::vector<int> colr;
    Player(float _x, float _y, float _radius, int _borderX, int _borderY, std::vector<int> _colr) : radius(_radius), borderX(_borderX), borderY(_borderY)
    {
        player.setRadius(_radius);
        position = {_x, _y};
        velocity = {0.f, 0.f};
        player.setPosition({position[0], position[1]});        
        colr = _colr;
        player.setFillColor(sf::Color(colr[0],colr[1],colr[2]));
    }

    Player(const Player &other)
    {
        position = other.position;
        velocity = other.velocity;
        radius = other.radius;
        colr = other.colr;
        borderX = other.borderX;
        borderY = other.borderY;
        player.setRadius(radius);
        player.setPosition({position[0], position[1]});
        player.setFillColor(sf::Color(colr[0],colr[1],colr[2]));
    }

    void Update(float _x, float _y)
    {        
        // update the velocity
        velocity[0] *= 1; velocity[1] *= 1;
        velocity[0] += _x; velocity[1] += _y;
        if ((position[0] < 0) || (position[0] + radius * 2 > borderX))
        {
            position[0] = (position[0] < 0) ? radius : (float)borderX - 2 * radius;
            velocity[0] = -velocity[0];
        }
        if ((position[1] < 0) || (position[1] + radius * 2 > borderY))
        {
            position[1] = (position[1] < 0) ? radius : (float)borderY - 2 * radius;
            velocity[1] = -velocity[1];
        }
        position[0] += velocity[0];
        position[1] += velocity[1];
        player.setPosition({position[0], position[1]});
    }
};

class NewEnvironementNoName
{
int numberOfPlayer;
std::vector<float> targetCireclePos;
public:
    sf::RenderWindow window;
    sf::CircleShape targetCirecle;
    std::vector<Player> players;
    NewEnvironementNoName(int _numberOfPlayer) : numberOfPlayer(_numberOfPlayer), window(sf::VideoMode({800, 600}), "Pakad Pakad MC", sf::Style::Default, sf::State::Windowed)
    {
        // set window
        window.setFramerateLimit(60);

        // Set player
        for(int i = 0; i < numberOfPlayer; i++)
        {
            float posX = GiveRandomFloat(0, 800), posY = GiveRandomFloat(0, 600);
            std::vector<int> colr = {GiveRandomInt(0, 255), GiveRandomInt(0, 255), GiveRandomInt(0, 255)};
            Player pl(posX, posY, 20, 800, 600, colr);
            players.push_back(pl);
        }

        // Set target points
        float posX = GiveRandomFloat(0, 800), posY = GiveRandomFloat(0, 600);

        targetCirecle.setRadius(20);
        targetCireclePos = {posX, posY};
        targetCirecle.setPosition({targetCireclePos[0], targetCireclePos[1]});
        targetCirecle.setFillColor(sf::Color(0, 0, 255));        
    }

    bool TakeTarget(int playerIndex)
    {
        // check if target take by the player
        // 20 is radius of target cirecle point
        float dx = (players[playerIndex].position[0] + players[playerIndex].radius) - (targetCireclePos[0] + 20);
        float dy = (players[playerIndex].position[1] + players[playerIndex].radius) - (targetCireclePos[1] + 20);
        float distanceSq = dx * dx + dy * dy;
        float radiusSum = players[playerIndex].radius + 20;
        return distanceSq <= radiusSum * radiusSum;
    }

    float Step(int action, int playerIndex)
    {
        // Complete action on particular player

         /*
        (Input)
            ACtion (0 - 7)
                -> 0: up
                -> 1: down
                -> 2: left
                -> 3: right
                -> 4: top left
                -> 5: top right
                -> 6: bottom left
                -> 7: bottom right
        (Return)
            Reward: float (1 / 0)
        */
        float speed = 0.5;
        if (action == 0)
            players[playerIndex].Update(0, speed);
        else if (action == 1)
            players[playerIndex].Update(0, -speed);
        else if (action == 2)
            players[playerIndex].Update(-speed, 0);
        else if (action == 3)
            players[playerIndex].Update(speed, 0);
        else if (action == 4)
            players[playerIndex].Update(-speed, speed);
        else if (action == 5)
            players[playerIndex].Update(speed, speed);
        else if (action == 6)
            players[playerIndex].Update(-speed, -speed);
        else if (action == 7)
            players[playerIndex].Update(speed, -speed);
        else if (action == 8)
            players[playerIndex].Update(0, 0);
        if(TakeTarget(playerIndex))
        {
            float posX = GiveRandomFloat(0, 800), posY = GiveRandomFloat(0, 600);
            targetCireclePos = {posX, posY};
            targetCirecle.setPosition({targetCireclePos[0], targetCireclePos[1]});
            return 1;
        }
        return 0;
    }
};