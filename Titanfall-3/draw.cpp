#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include <FL/Enumerations.H>

#include "draw.h"
#include "level_gen.h"
#include "settings.h"

using namespace Graph_lib;
Graph_lib::Open_polyline *fn2 = nullptr;

void game_draw(Screen &main_win, std::vector<Player> &pl,
               std::vector<Obstacle> &obst, input_data equation, Player real_player)
{ // TODO: add icon
    std::pair<int, int> q = real_player.GetCords();

    std::vector<std::pair<int, int>> l;
    // obstacles
    Vector_ref<Circle *> show_obs;
    // players
    Vector_ref<Circle *> show_pl;
    // nicknames
    Vector_ref<Text *> show_nm;
    // draw obstacles
    for (size_t i = 0; i < obst.size(); ++i)
    {
        Circle *c = new Circle{obst[i].center, obst[i].radius};
        if (obst[i].hole)
        {
            c->set_fill_color(FL_LIGHT1);
            c->set_color(FL_LIGHT1);
        }
        else
            c->set_fill_color(HugeObsColor);

        show_obs.push_back(c);
        main_win.attach(*show_obs[i]);
    };
    // draw players
    for (size_t i{0}; i < pl.size(); ++i)
    {
        Circle *c = new Circle{Point(pl[i].GetCords().first, pl[i].GetCords().second), PlayerRad};
        if (pl[i].IsAlive())
        {
            if (pl[i].GetName() == real_player.GetName())
            {
                c->set_fill_color(Graph_lib::Color::yellow);
            }
            else
            {
                c->set_fill_color(Graph_lib::Color::cyan);
            }
        }
        else
        {
            c->set_fill_color(Graph_lib::Color::dark_red);
        }
        show_pl.push_back(c);
        main_win.attach(*show_pl[i]);
    };
    // draw nicknames
    for (size_t i{0}; i < pl.size(); ++i)
    {
        Text *c{nullptr};
        if (pl[i].GetCords().second + PlayerRad >= FieldHeight)
        {
            c = new Text{Point(pl[i].GetCords().first, pl[i].GetCords().second + PlayerRad),
                         pl[i].GetName()};
        }
        else
        {
            c = new Text{Point(pl[i].GetCords().first, pl[i].GetCords().second - PlayerRad),
                         pl[i].GetName()};
        }
        c->set_font_size(NickSize);
        show_nm.push_back(c);
        main_win.attach(*show_nm[i]);
    };
    std::string function = equation.equation;
    // draw function
    main_win.detach(*fn2);
    fn2 = new Graph_lib::Open_polyline;
    l = Func_trace(function, q, obst, pl, equation.right);
    for (auto i : l)
    {
        Point *j = new Point(i.first, i.second);
        fn2->add(*j);
    }
    fn2->set_color(Graph_lib::Color::red);
    main_win.attach(*fn2);
}

void func_draw(Screen &main_win, std::vector<Point> &f)
{ // TODO: add icon
    Graph_lib::Open_polyline fn2;
    // create & draw function
    for (auto i : f)
    {
        Point *j = new Point(i.x, i.y);
        fn2.add(*j);
    }
    fn2.set_color(Graph_lib::Color::red);
    main_win.attach(fn2);
}
