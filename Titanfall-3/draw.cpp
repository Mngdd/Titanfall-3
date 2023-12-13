#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>

#include "draw.h"
#include "level_gen.h"
#include "settings.h"

using namespace Graph_lib;

Graph_lib::Open_polyline fn;
void game_draw(Screen &main_win, std::vector<Player> &pl,
               std::vector<Obstacle> &obst)
{ // TODO: add icon
    // функция текстом
    // std::string function = "120 * sin(x/2)";
    std::pair<int, int> q = std::make_pair(100, 100);
    std::vector<std::pair<int, int>> l;
    // наша функция, ее тело
    // препятствия
    Vector_ref<Circle *> show_obs;
    // игроки (тела, странно почему мы так их храим...)
    Vector_ref<Circle *> show_pl;
    // ники
    Vector_ref<Text *> show_nm;
    // main_win.redraw_label();
    std::string s = "x*100";
    bool right = true;
    l = Func_trace(s, q, obst, pl, right);
    for (auto i : l)
    {
        Point *j = new Point(i.first, i.second);
        fn.add(*j);
    }
    fn.set_color(Graph_lib::Color::red);
    main_win.redraw();

    // draw obstacles
    for (size_t i = 0; i < obst.size(); ++i)
    {
        Circle *c = new Circle{obst[i].center, obst[i].radius};
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
    main_win.attach(fn);
    // main_win.control_win.shown();

    input_data equation = main_win.control_win.wait_for_game_button();

    std::string function = equation.equation;
    std::cout << "666:)" << function << '\n';

    // create & draw function
    // main_win.detach(fn);
    // fn.~Shape();

    if (equation.g_s == game_state::FIRE)
    {
        main_win.detach(fn);
        Graph_lib::Open_polyline *fn2 = new Graph_lib::Open_polyline;

        l = Func_trace(function, q, obst, pl, equation.right);
        for (auto i : l)
        {
            Point *j = new Point(i.first, i.second);
            fn2->add(*j);
            std::cout << j->x << ' ' << j->y << '\n';
        }
        fn2->set_color(Graph_lib::Color::red);
        main_win.redraw();
        main_win.attach(*fn2);
    }

    // if (equation.g_s == game_state::FIRE)
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
