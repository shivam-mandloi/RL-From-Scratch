#include "Tag.hpp"
#include "DeeepQLearning.hpp"

/*
    problem: Tag simulation not able to find the collision properly, we are only checking collison after elapsed time
*/

int main()
{
    int totalTime = 120, crnTime = 60;
    sf::Clock clock;
    float elapsedTime = 0.f;

    float terminalRward = 100; // used as positive reward, wither red get or blue

    // Tag Environment
    TAG tag(totalTime);
    tag.window.setFramerateLimit(60);
    
    // Q Learning
    DeepQLearning playerRed, playerBlue;

    while (tag.window.isOpen())
    {
        while (const std::optional event = tag.window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                tag.window.close();
        }

        float deltaTime = clock.restart().asSeconds();
        elapsedTime += deltaTime;

        if (elapsedTime >= 0.2f)
        {
            elapsedTime = 0.f;
            vecX<double> state = {tag.p1.position[0], tag.p1.position[1], tag.p1.velocity[0], tag.p1.velocity[1], tag.p2.position[0], tag.p2.position[1], tag.p2.velocity[0], tag.p2.velocity[1], (double)crnTime};
            state.TR(); // Q learning take column vector
            int redAction = playerRed.GetNextAction(state);
            int blueAction = playerBlue.GetNextAction(state);
            crnTime-=1;

            float rewardRed = tag.PerFormAction(redAction, RED);
            float rewardBlue = tag.PerFormAction(blueAction, BLUE);
            tag.Update();
            
            vecX<double> nextState = {tag.p1.position[0], tag.p1.position[1], tag.p1.velocity[0], tag.p1.velocity[1], tag.p2.position[0], tag.p2.position[1], tag.p2.velocity[0], tag.p2.velocity[1], (double)crnTime};
            nextState.TR(); // Q learning take column vector

            if (tag.isCatch())
            {
                std::cout << "catch" << std::endl;
                rewardRed += terminalRward;
                rewardBlue -= terminalRward;
            }
            if (tag.isTimeLimit())
            {
                std::cout << "time limit" << std::endl;
                rewardRed -= terminalRward;
                rewardBlue += terminalRward;
            }

            std::cout << "Red Action: " << redAction << " | Red Reward: " << rewardRed << " | Blue Action: " << blueAction << " | Blue Reward: " << rewardBlue << std::endl;
            playerRed.SaveState(state, redAction, nextState, rewardRed, tag.isCatch() || tag.isTimeLimit());
            playerBlue.SaveState(state, blueAction, nextState, rewardBlue, tag.isCatch() || tag.isTimeLimit());

            double lossBlue = playerBlue.Learn();
            double lossRed = playerRed.Learn();
            std::cout << "Blue Player Learning Loss: " << lossBlue << " | Red Player Learning Loss: " << lossRed << std::endl;
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
            float rewardRed = -terminalRward;
            if(tag.isCatch())
                rewardRed = terminalRward;
            vecX<double> state = {tag.p1.position[0], tag.p1.position[1], tag.p1.velocity[0], tag.p1.velocity[1], tag.p2.position[0], tag.p2.position[1], tag.p2.velocity[0], tag.p2.velocity[1], (double)crnTime};
            playerRed.SaveState(state, 0, state, rewardRed, true); // next state is random
            playerBlue.SaveState(state, 0, state, rewardRed * -1, true); // next state is random
            std::cout << "Blue Total Reward: " << playerBlue.totalReward << " | Red Total Reward: " << playerRed.totalReward << std::endl;
            double lossBlue = playerBlue.Learn();
            double lossRed = playerRed.Learn();
            std::cout << "Blue Player Learning Loss: " << lossBlue << " | Red Player Learning Loss: " << lossRed << std::endl;
            playerBlue.totalReward = 0; playerRed.totalReward = 0;
            tag.ResetPlayers();
            crnTime = totalTime;
        }

        tag.Update();
        tag.window.clear();

        tag.window.draw(tag.p1.shape);
        tag.window.draw(tag.p2.shape);

        tag.window.display();
    }

    return 0;
}