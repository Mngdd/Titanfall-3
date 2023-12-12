#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>

#include "draw.h"
#include "level_gen.h"
#include "settings.h"

using namespace Graph_lib;
// препятствия
Vector_ref<Circle *> show_obs;
// игроки (тела, странно почему мы так их храим...)
Vector_ref<Circle *> show_pl;
// ники
Vector_ref<Text *> show_nm;
// игроки как клаасс. опять же, странно храним, могли бы 1 такой список держать
// std::vector<Player> pl;// делать указатели юзлесс
// наша функция, ее тело
Graph_lib::Open_polyline fn;

Graph_lib::Open_polyline fn2;

// лучше названия не придумаешь, хз че
std::vector<std::pair<int, int>> l;

// std::vector<Obstacle *> Obstacles;
// std::vector<Point *> Players;
// inline std::vector<std::pair<int, int>> func;
// bool try;
// очень ясно, 100% понимания, 0% осуждения
std::pair<int, int> q = std::make_pair(100, 100);
// функция текстом
std::string function = "120 * sin(x/2)";

void game_draw(Screen &main_win, std::vector<Player> &pl)
{ // TODO: add icon
    // create & draw function
    // l = Func_trace(function, q, Obstacles, Players);
    // for (auto i : l)
    // {
    //     Point *j = new Point(i.first, i.second);
    //     fn.add(*j);
    // }
    // fn.set_color(Graph_lib::Color::red);

    // draw obstacles
    for (size_t i = 0; i < Obstacles.size(); ++i)
    {
        Circle *c = new Circle{Obstacles[i].center, Obstacles[i].radius};
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
            c->set_fill_color(Graph_lib::Color::cyan);
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
        if (pl[i].GetCords().second + PlayerRad >= FieldHeight) {
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
    main_win.attach(fn);
}

void func_draw(Screen &main_win, std::vector<Point> &f)
{ // TODO: add icon
    // create & draw function
    for (auto i : f)
    {
        Point *j = new Point(i.x, i.y);
        fn2.add(*j);
    }
    fn2.set_color(Graph_lib::Color::red);
    main_win.attach(fn2);
}
