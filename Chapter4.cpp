#include "BSW.hpp"
#include "PureExploitation.hpp"
#include "PureExploration.hpp"

using namespace std;

int main()
{
    vect<float> prob = {0.2, 0.8};
    BSW bsw(prob);
    
    vect<float> value = PureExploitation(1000, prob);
    cout << "Pure Exploitation: " << endl;
    value.print();

    value = PureExploration(1000, prob);
    cout << "Pure Exploration: " << endl;
    value.print();

    return 0;
}