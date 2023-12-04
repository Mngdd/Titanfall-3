// у меня не робят закоменченные инклюды лол
#include <Graph_lib/Simple_window.h>
#include <Graph_lib/Graph.h>

#include "level_gen.h"
#include "settings.h"
#include "enviroment.h"
#include "graphics.h"
#include <vector>

using namespace Graph_lib;
std::vector<Circle *> show_obs;
std::vector<Circle *> show_pl;
std::vector<Text *> show_nm;
std::vector<Player *> pl;

void drawing()
{

    Generate(); // generation obstacles and players
    std::cout << Obstacles.empty();

    Game_window win{Point(100, 100), FieldLength, FieldWidth, "Window"};

    // for (size_t i{0}; i < q.size(); ++i)
    // {
    // Point *m = new Point(100, 100);
    // Players.push_back(m);
    // }

    win.wait_for_button();

    // for (size_t i{0}; i < Obstacles.size(); ++i) // draw obstacles
    // {
    //     Circle *c = new Circle{Obstacles[i]->center, Obstacles[i]->radius};
    //     c->set_fill_color(HugeObsColor);
    //     show_obs.push_back(c);
    // };

    // for (auto i : show_obs)
    // {
    //     win.attach(*i);
    // }

    // for (size_t i{0}; i < pl.size(); ++i) // draw obstacles
    // {
    //     Circle *c = new Circle{Point(pl[i]->GetCords().first, pl[i]->GetCords().second), PlayerRad};
    //     c->set_fill_color(PlayerColor);
    //     show_pl.push_back(c);
    // };

    // for (auto i : show_pl)
    // {
    //     win.attach(*i);
    // }

    win.wait_for_button();

    win.wait_for_button();
}