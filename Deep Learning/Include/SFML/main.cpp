#include <SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>

const int GRID_W = 10;
const int GRID_H = 10;
const int CELL_SIZE = 40;

// using namespace sf;

int main() {
    // sf::RenderWindow window(sf::VideoMode(640, 480), "Temp", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Ye Jo desh hai mera", sf::Style::Default, sf::State::Windowed);

    window.setFramerateLimit(120);
    
    //circle
    sf::CircleShape shape(20.f);
    shape.setFillColor(sf::Color(100, 250, 50));
    shape.setPosition({4.f, 5.f});


    //Rectangle
    sf::RectangleShape rect({20.f, 20.f});
    rect.setPosition({80, 40});

    float x = 0, y = 0;

    while(window.isOpen())
    {
        while(const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
                window.close();

            else if(auto* e = event->getIf<sf::Event::MouseButtonPressed>())
            {
                x = e->position.x; y = e->position.y;
                shape.setPosition(sf::Vector2f(e->position.x, e->position.y));
            }
        }
        
        window.clear();
        window.draw(shape);
        window.draw(rect);
        x ++; y ++;
        if(x > 800)
            x = 0;
        if(y > 600)
            y = 0;
        shape.setPosition({x, y});
        rect.setPosition({80, y});

        window.display();
    }
    return 0;
}