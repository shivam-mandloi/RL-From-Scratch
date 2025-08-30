#include "GridWorld.hpp"


using namespace std;

int main()
{
    vector<int> pit = {7, 17, 21};
    GridWorld worl(pit);
    State player(0,1);
    worl.Show(player);
    cout << worl.GetReward(player, 0) << endl;
    cout << worl.GetReward(player, 1) << endl;
    cout << worl.GetReward(player, 2) << endl;
    cout << worl.GetReward(player, 3) << endl;
    return 0;
}