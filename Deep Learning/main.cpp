#include "Tag.hpp"

int main()
{
    TAG tag;
    tag.window.setFramerateLimit(60);
    sf::Clock clock;
    float elapsedTime = 0.f; 

    while (tag.window.isOpen())
    {
        while (const std::optional event = tag.window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                tag.window.close();
        }

        float deltaTime = clock.restart().asSeconds();
        elapsedTime += deltaTime;

        if (elapsedTime >= 1.0f)
        {
            elapsedTime = 0.f;
            std::cout << "Performing action every 1 second...\n";
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            tag.PlayerForce(0, -0.1, BLUE);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            tag.PlayerForce(0, 0.1, BLUE);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            tag.PlayerForce(-0.1, 0, BLUE);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            tag.PlayerForce(0.1, 0, BLUE);

        if(tag.isCatch())
        {
            std::cout << "MC Caught" << std::endl;
            tag.window.close();
        }

        tag.window.clear();
        
        tag.Update();
        tag.window.draw(tag.p1.shape);
        tag.window.draw(tag.p2.shape);        

        tag.window.display();
    }

    return 0;
}