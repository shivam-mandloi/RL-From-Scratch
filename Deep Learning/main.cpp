#include "Tag.hpp"
#include "DeepMonteCarlo.hpp"

int main()
{
    TAG tag;
    tag.window.setFramerateLimit(60);
    sf::Clock clock;
    float elapsedTime = 0.f;
    DeepMonteCarlo playerRed, playerBlue;

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
            vecX<double> state = {tag.p1.position[0], tag.p1.position[1], tag.p1.velocity[0], tag.p1.velocity[1], tag.p2.position[0], tag.p2.position[1], tag.p2.velocity[0], tag.p2.velocity[1]};
            int redAction = playerRed.GetNextAction(state);
            int blueAction = playerBlue.GetNextAction(state);

            float rewardRed = tag.PerFormAction(redAction, RED);
            float rewardBlue = tag.PerFormAction(blueAction, BLUE);
            if (tag.isCatch())
            {
                rewardRed += 1000;
                rewardBlue += (-1000);
            }
            if (tag.isTimeLimit())
            {
                rewardRed += (-1000);
                rewardBlue += 1000;
            }
            std::cout << "Red Action: " << redAction << " Red Reward: " << rewardRed << " Blue Action: " << blueAction << " Blue Reward: " << rewardBlue << std::endl;
            playerRed.SaveState(state, rewardRed, redAction);
            playerBlue.SaveState(state, rewardBlue, blueAction);
        }

        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        //     tag.PlayerForce(0, -0.1, BLUE);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        //     tag.PlayerForce(0, 0.1, BLUE);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        //     tag.PlayerForce(-0.1, 0, BLUE);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        //     tag.PlayerForce(0.1, 0, BLUE);

        if (tag.isCatch() || tag.isTimeLimit())
        {
            std::cout << "MC Caught" << std::endl;
            std::cout << "Blue Total Reward: " << playerBlue.totalReward << " Red Total Reward: " << playerRed.totalReward << std::endl;
            double lossBlue = playerBlue.Learn();
            double lossRed = playerRed.Learn();
            std::cout << "Blue Player: " << lossBlue << " Red Player: " << lossRed << std::endl;
            tag.ResetPlayers();
            // continue;
            // tag.window.close();
        }

        tag.window.clear();

        tag.Update();
        tag.window.draw(tag.p1.shape);
        tag.window.draw(tag.p2.shape);

        tag.window.display();
    }

    return 0;
}