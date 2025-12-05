#include "NewEnvironementNoName.hpp"
// #include "DeeepQLearning.hpp"
#include "NetworkForValueFunction.hpp"

/*
    
*/

class DeepMonteCarlo
{
    NetworkValue player;
    int randCount = 0;
    std::vector<vecX<double>> allState;
    std::vector<int> selectedAction;
    std::vector<float> allReward;

public:
    float epsD, totalReward = 0.f, gamma;
    DeepMonteCarlo(float _epsD = 0.1, float _gamma = 0.99) : epsD(_epsD), gamma(_gamma) {}

    int SelectAction(vecX<double> &predictValue)
    {
        // Select Action based on epsilon greedy method
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        double randomNumber = distr(gen);
        if (randomNumber <= epsD)
        {
            std::uniform_int_distribution<> uniformInt(0, 8);
            return uniformInt(gen);
        }
        int index = 0;
        for (int i = 1; i < predictValue.len; i++)
        {
            if (predictValue.Get(index) < predictValue.Get(i))
                index = i;
        }
        return index;
    }

    // main function
    int GetNextAction(vecX<double> state)
    {
        /*
            Main Function take state as input
            state: dim(4)
                -> first two input element is position of player 1
                -> last two input element is position of player 2
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
        state.TR();
        vecX<double> predictValue = player.PredictValueFunction(state);
        return SelectAction(predictValue);
    }

    // Take input and actual
    double Learn()
    {
        vecX<double> state = ConvertVectorToVecX(allState);
        state.TR();
        vecX<double> target = state;
        float discntReward = 0;
        for(int i = state.col - 1; i > -1; i--)
        {
            discntReward = allReward[i] + discntReward * gamma;
            target.push(i, selectedAction[i], discntReward);
        }
        return player.Learn(state, target);
    }

    // save each state, based on selected action and result from the environment
    void SaveState(vecX<double> state, float reward, int action)
    {
        allState.push_back(state);        
        allReward.push_back(reward);
        selectedAction.push_back(action);
    }

    void Reset()
    {
       allState = {}; allReward = {}, selectedAction = {}; 
    }
};

int main()
{        
    NewEnvironementNoName noName(1);
    sf::Clock clock;
    float elapsedTime = 0.f, totalRewardSum = 0; 
    int totalTime = 60, crnTime = 60;
    DeepMonteCarlo agent;

    while (noName.window.isOpen())
    {
        while (const std::optional event = noName.window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                noName.window.close();
        }

        float deltaTime = clock.restart().asSeconds();
        elapsedTime += deltaTime;

        if (elapsedTime >= 0.2f)
        {
            elapsedTime = 0.f;
            vecX<double> state = {noName.players[0].position[0], noName.players[0].position[1], noName.targetCireclePos[0], noName.targetCireclePos[1], (double) crnTime};
            crnTime -= 1;
            int action = agent.GetNextAction(state);
            float reward = noName.Step(action, 0);
            totalRewardSum += reward;
            agent.SaveState(state, reward, action);
            if(!crnTime)
            {
                double loss = agent.Learn();
                agent.Reset();
                crnTime = totalTime;
                std::cout << "Loss: " << loss << " Total Reward: " << totalRewardSum << std::endl;
                totalRewardSum = 0;
            }
        }

        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        //     noName.Step(1, 0);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        //     noName.Step(0, 0);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        //     noName.Step(2, 0);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        //     noName.Step(3, 0);

        noName.window.clear();

        for(int i = 0; i < noName.players.size(); i++)
        {
            float reward = noName.Step(8, i);
            if(reward)
            {
                vecX<double> state = {noName.players[0].position[0], noName.players[0].position[1], noName.targetCireclePos[0], noName.targetCireclePos[1], (double) crnTime};
                agent.SaveState(state, reward, 8);
                totalRewardSum += reward;
            }            
            noName.window.draw(noName.players[i].player);
        }
        noName.window.draw(noName.targetCirecle);

        noName.window.display();
    }

    return 0;
}