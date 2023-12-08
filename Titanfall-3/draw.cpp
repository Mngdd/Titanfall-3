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
Graph_lib::Open_polyline fn;
std::vector<std::pair<int, int>> l;
std::vector<std::pair<int, int>> Func_trace(std::string func_enter, std::pair<int, int> &cords, std::vector<Obstacle *> obstacle_mini, std::vector<Point *> players_cords);
// std::vector<Obstacle *> Obstacles;
// std::vector<Point *> Players;
// inline std::vector<std::pair<int, int>> func;
// bool try;
std::pair<int, int> q = std::make_pair(100,100); 
std::string function = "120 * sin(x/2)";
void drawing()
{

    Generate(); // generation obstacles and players

    Game_window win{Point(350, 500), FieldLength, FieldWidth, "Window"};

    win.wait_for_button();
    std::string *n = new std::string{"NoName"};
    pl_name.push_back(n);

    l = Func_trace(function, q, Obstacles, Players);

    for (auto i : l){
        Point* j = new Point(i.first, i.second);
        fn.add(*j);
    }
    fn.set_color(Graph_lib::Color::yellow);
    // for (auto i : fn){
    //     win
    // }

    for (size_t i{0}; i < Players.size(); ++i)
    {
        Player *m = new Player{*pl_name[i], Players[i]->x, Players[i]->y};
        pl.push_back(m);
    }

    for (size_t i{0}; i < Obstacles.size(); ++i) // draw obstacles
    {
        Circle *c = new Circle{Obstacles[i]->center, Obstacles[i]->radius};
        c->set_fill_color(Graph_lib::Color::black);
        show_obs.push_back(c);
    };

    for (auto i : show_obs)
    {
        win.attach(*i);
    }

    for (size_t i{0}; i < pl.size(); ++i) // draw pl
    {
        Circle *c = new Circle{Point(pl[i]->GetCords().first, pl[i]->GetCords().second), PlayerRad};
        c->set_fill_color(Graph_lib::Color::cyan);
        show_pl.push_back(c);
    };

    for (auto i : show_pl)
    {
        win.attach(*i);
    }

    for (size_t i{0}; i < pl.size(); ++i) // draw nm
    {
        Text *c{nullptr};
        if (pl[i]->GetCords().second + PlayerRad >= FieldWidth)
            c = new Text{Point(pl[i]->GetCords().first, pl[i]->GetCords().second + PlayerRad), pl[i]->GetName()};
        else
            c = new Text{Point(pl[i]->GetCords().first, pl[i]->GetCords().second - PlayerRad), pl[i]->GetName()};
        c->set_font_size(20);
        show_nm.push_back(c);
    };

    for (auto i : show_nm)
    {
        win.attach(*i);
    }

    win.attach(fn);
    win.wait_for_button();

    win.wait_for_button();
}