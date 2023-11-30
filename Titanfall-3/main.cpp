// у меня не робят закоменченные инклюды лол
#include "level_gen.h"
#include "settings.h"
//
#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
// #include "server.h"
// #include "client.h"
using namespace Graph_lib;

int main ()
{
    HugeObsSpawn();
    MediumObsSpawn();
    SmallObsSpawn();  // adds obstacles to SmallObstacles
    PlayersSpawn();

    Simple_window win{Point(100, 100), FieldLength, FieldWidth, "Window"};
    for (int i{0}; i < HugeObstacles.size(); ++i)
    {
        win.attach(*HugeObstacles[i]);
    };

    for (int i{0}; i < MediumObstacles.size(); ++i)
    {
        win.attach(*MediumObstacles[i]);
    };

    for (int i{0}; i < Players.size(); ++i)
    {
        win.attach(*Players[i]);
    };
    win.wait_for_button();
}