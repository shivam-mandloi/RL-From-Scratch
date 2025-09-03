#include "BSW.hpp"
#include "PureExploitation.hpp"
#include "PureExploration.hpp"
#include "EpsilonGreedy.hpp"

using namespace std;

int main()
{
    vect<float> prob = {0.2, 0.8, 0.3, 0.9, 0.5};
    BSW bsw(prob);
    
    vect<float> value = PureExploitation(1000, prob);
    cout << "Pure Exploitation: " << endl;
    value.print();

    value = PureExploration(1000, prob);
    cout << "Pure Exploration: " << endl;
    value.print();

    value = EpsilonGreedy(1000, prob, 0.2);
    cout << "Epsilon Greedy: " << endl;
    value.print();

    return 0;
}