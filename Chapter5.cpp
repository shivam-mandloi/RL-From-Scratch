#include "RW.hpp"
#include "MonteCarlo.hpp"
#include "TDPrediction.hpp"

using namespace std;

int main()
{
    vect<float> prob = {0.8, 1, 0.9, 0.6, 0.9, 0.4, 0.7, 0.8, 1, 0.6};
    prob.row = 5;
    prob.col = 2;

    prob.print();
    RW rw(prob);

    vect<int> pi(1, 5, 1);
    vect<float> value = MonteCarlo(prob, pi, 1000, 0.9, 0.2);
    value.print();

    value = TDMethod(prob, pi, 0.9, 1000, 0.3);
    value.print();
    return 0;
}