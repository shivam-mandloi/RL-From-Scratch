#include "GridWorld.hpp"
#include "PolicyIteration.hpp"


using namespace std;

int main()
{
    vector<int> pit = {7, 17, 9, 3, 15, 21};
    
    GridWorld worl(pit);
    worl.Show(State(0, 0));
    
    vector<vector<int>> policy = PolicyIteration(pit, 0.9, 0.1);
    for(auto vec : policy)
    {
        for(auto val : vec)
        {
            cout << val << " ";
        }
        cout << endl;
    }
    cout << endl;

    return 0;
}