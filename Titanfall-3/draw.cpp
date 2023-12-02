// у меня не робят закоменченные инклюды лол
#include "level_gen.h"
#include "settings.h"
#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include "enviroment.h"
#include "graphics.h"
using namespace Graph_lib;

int drawing()
{

    HugeObsSpawn();
    MediumObsSpawn();
    SmallObsSpawn(); // adds obstacles to SmallObstacles
    PlayersSpawn();

    Game_window win{Point(100, 100), FieldLength, FieldWidth, "Window"};

    win.wait_for_button();

    for (size_t i{0}; i < pl.size(); ++i)
    {
        // Out_box *o = new Out_box{*Players[i]->center(), 70, 40, pl[i]};
        Text *o;
        if ((*Players[i]).center().y + 40 < 1200)
            o = new Text{Point((*Players[i]).center().x, (*Players[i]).center().y + 40), pl[i].GetName()};
        else
            o = new Text{Point((*Players[i]).center().x, (*Players[i]).center().y - 40), pl[i].GetName()};
        o->set_font_size(30);
        win.attach(*o);
    }

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