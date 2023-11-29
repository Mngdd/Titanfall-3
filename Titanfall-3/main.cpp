<<<<<<< Updated upstream

#include "level_gen.h"
#include "main.h"

#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>

using namespace Graph_lib;

int main()
try
{
    Point t1{100,100};
    Simple_window win{t1, 600, 400, "Canvas"}; // как сделать пустое поле без простейшей кнопки? и что означает первая корда?
=======
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
    int x;
    x = rand();

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
>>>>>>> Stashed changes
}