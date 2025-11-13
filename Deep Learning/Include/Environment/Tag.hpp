/*
    Two Agents in one environement, One has to tag other agent and other has job to avoid to get tag
    Reward:
        Suppose red has to find and blue has to hide
        we run for some time step, suppose for 60, (1 time step -> 1 sec)
        For Blue:
            -> +1 for each time step
            -> +1000 if not get caught
            -> -1000 if caught by red (game closed)
        For Red:
            -> -1 for each time step
            -> +1000 if catch blue (game closed)
            -> -1000 if not able to catch blue in given time (game closed)
*/

#pragma once

#include "IncludeSFML.hpp"
#include <iostream>
#include <algorithm>

enum PlayerType
{
    BLUE,
    RED
};

class Player
{
    int borderX, borderY;
    
public:
    std::vector<float> position, velocity;
    sf::CircleShape shape;
    PlayerType type;
    float radius;
    Player(float _x, float _y, PlayerType _type, float _radius, int _borderX, int _borderY) : radius(_radius), type(_type), borderX(_borderX), borderY(_borderY)
    {
        shape.setRadius(_radius);
        position = {_x, _y};
        velocity = {0.f, 0.f};
        shape.setPosition({position[0], position[1]});
        if (type == RED)
            shape.setFillColor(sf::Color(255, 0, 0));
        else
            shape.setFillColor(sf::Color(0, 0, 255));
    }

    void Update()
    {
        if ((position[0] < 0) || (position[0] + radius * 2 > borderX))
        {
            position[0] = (position[0] < 0) ? radius : (float)borderX -  2 * radius;
            velocity[0] = -velocity[0];
        }
        if ((position[1] < 0 ) || (position[1] + radius * 2 > borderY))        
        {
            position[1] = (position[1] < 0) ? radius : (float)borderY - 2 * radius;
            velocity[1] = -velocity[1];
        }
        position[0] += velocity[0];
        position[1] += velocity[1];
        shape.setPosition({position[0], position[1]});
    }

    void UpdateVelocity(float _x, float _y)
    {
        velocity[0] += _x;
        velocity[1] += _y;
    }
};

class TAG
{
public:
    sf::RenderWindow window;
    Player p1, p2;
    // Fixed the size of window
    TAG() : window(sf::VideoMode({800, 600}), "Pakad Pakad MC", sf::Style::Default, sf::State::Windowed), p1(0, 300, BLUE, 10, 800, 600), p2(700, 300, RED, 10, 800, 600){}

    void PlayerForce(float xDirection, float yDirection, PlayerType type)
    {
        if(type == BLUE)
            p1.UpdateVelocity(xDirection, yDirection);
        else
            p2.UpdateVelocity(xDirection, yDirection);        
    }

    void Update()
    {
        p1.Update();
        p2.Update();
    }

    bool isCatch()
    {        
        float dx = (p1.position[0] + p1.radius) - (p2.position[0] + p2.radius);
        float dy = (p1.position[1] + p1.radius) - (p2.position[1] + p2.radius);    
        float distanceSq = dx * dx + dy * dy;
        float radiusSum = p1.radius + p2.radius;
        return distanceSq <= radiusSum * radiusSum;
    }
};