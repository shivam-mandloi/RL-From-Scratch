#include "NewEnvironementNoName.hpp"
#include "DeeepQLearning.hpp"

/*
    problem: Tag simulation not able to find the collision properly, we are only checking collison after elapsed time
*/

int main()
{        
    NewEnvironementNoName noName(1);    

    while (noName.window.isOpen())
    {
        while (const std::optional event = noName.window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                noName.window.close();
        }        

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            noName.Step(1, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            noName.Step(0, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            noName.Step(2, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            noName.Step(3, 0);

        // noName.Update();
        noName.window.clear();

        for(int i = 0; i < noName.players.size(); i++)
        {
            noName.Step(8, i);
            noName.window.draw(noName.players[i].player);
        }
        noName.window.draw(noName.targetCirecle);

        noName.window.display();
    }

    return 0;
}