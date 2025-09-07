#include "vect.hpp"
#include "MCControl.hpp"
#include "SARSA.hpp"

using namespace std;

void PrintQValue(vector<vect<float>> Q)
{
    for (int i = 0; i < Q.size(); i++)
    {
        Q[i].print();
    }
}

int main()
{
    vect<float> prob = {0.8, 1, 0.9, 0.6, 0.9, 0.4, 0.7, 0.8, 1, 0.6};
    prob.row = 5;
    prob.col = 2;
    prob.print();
    vector<vect<float>> Q = MCControl(prob, 0.3, 0.9, 0.2, 3000);
    PrintQValue(Q);
    Q = SARSA(prob, 0.1, 0.9, 0.4, 3000, RWState(2));
    PrintQValue(Q);
    return 0;
}