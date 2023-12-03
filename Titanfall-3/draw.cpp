// у меня не робят закоменченные инклюды лол
#include <Graph_lib/Simple_window.h>
#include <Graph_lib/Graph.h>

#include "level_gen.h"
#include "settings.h"
#include "enviroment.h"
#include "graphics.h"

using namespace Graph_lib;

void drawing()
{

    Generate(); // generation obstacles and players

    Game_window win{Point(100, 100), FieldLength, FieldWidth, "Window"};

    win.wait_for_button();

    for (size_t i{0}; i < pl.size(); ++i) // draw names
    {
        Text *o;
        if ((*Players[i]).y + 40 < 1200)
            o = new Text{Point((*Players[i]).x, (*Players[i]).y + 40), pl[i]->GetName()};
        else
            o = new Text{Point((*Players[i]).x, (*Players[i]).y - 40), pl[i]->GetName()};
        o->set_font_size(30);
        win.attach(*o);
    }

    for (size_t i{0}; i < Obstacles.size(); ++i) // draw obstacles
    {
        Circle *c = new Circle{Obstacles[i]->center, Obstacles[i]->radius};
        win.attach(*c);
    };

    for (size_t i{0}; i < Players.size(); ++i) // draw platers
    {
        Circle *c = new Circle{*Players[i], PlayerRad};
        win.attach(*c);
    };
    win.wait_for_button();
}