#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include <FL/Enumerations.H>

#include "draw.h"
#include "level_gen.h"
#include "settings.h"

using namespace Graph_lib;
Graph_lib::Open_polyline *fn2 = nullptr;
Vector_ref<Circle> show_obs;
Vector_ref<Circle> show_pl;
Vector_ref<Text> show_nm;

void game_draw(Screen &main_win, std::vector<Player> &pl,
               std::vector<Obstacle> &obst, input_data equation, Player real_player)
{ // TODO: add icon
    // очищаем вывод
    std::string s1 = "";
    main_win.control_win.print_text(s1);
    // координаты, того стреляет
    std::pair<int, int> q = real_player.GetCords();
    // сообщение об ошибке
    std::string s = "ERROR";
    // вектор точек графика
    std::vector<std::pair<int, int>> l;
    GenerationSettings genset = main_win.settings();
    // рисуем препятствия
    for (size_t i = 0; i < obst.size(); ++i)
    {
        if (!obst[i].hole)
        {
            Circle *c = new Circle{obst[i].center, obst[i].radius};
            c->set_fill_color(HugeObsColor);
            show_obs.push_back(c);
            main_win.attach(show_obs[i]);
        }
    };

    for (size_t i = 0; i < obst.size(); ++i)
    {
        if (obst[i].hole)
        {
            Circle *c = new Circle{obst[i].center, obst[i].radius};
            c->set_fill_color(Graph_lib::Color::white);
            show_obs.push_back(c);
            main_win.attach(show_obs[i]);
            // main_win.redraw();
        }
    }
    // рисуем игроков
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
        main_win.attach(show_pl[i]);
        Text *t{nullptr};
        if (pl[i].GetCords().second + PlayerRad >= FieldHeight)
        {
            t = new Text{Point(pl[i].GetCords().first, pl[i].GetCords().second + PlayerRad),
                         pl[i].GetName()};
        }
        else
        {
            t = new Text{Point(pl[i].GetCords().first, pl[i].GetCords().second - PlayerRad),
                         pl[i].GetName()};
        }
        t->set_font_size(NickSize);
        t->set_color(Graph_lib::Color::dark_magenta);
        show_nm.push_back(t);
        main_win.attach(show_nm[i]);
    };
    // печатаем имена игроков
    // for (size_t i{0}; i < pl.size(); ++i)
    // {
    //     Text *c{nullptr};
    //     if (pl[i].GetCords().second + PlayerRad >= FieldHeight)
    //     {
    //         c = new Text{Point(pl[i].GetCords().first, pl[i].GetCords().second + PlayerRad),
    //                      pl[i].GetName()};
    //     }
    //     else
    //     {
    //         c = new Text{Point(pl[i].GetCords().first, pl[i].GetCords().second - PlayerRad),
    //                      pl[i].GetName()};
    //     }
    //     c->set_font_size(NickSize);
    //     c->set_color(Graph_lib::Color::dark_magenta);
    //     show_nm.push_back(c);
    //     main_win.attach(show_nm[i]);
    // };
    std::string function = equation.equation;
    // рисуем функцию
    switch (equation.g_s)
    {
    // fire
    case (game_state::FIRE):
    {
        main_win.detach(*fn2);
        delete fn2;
        fn2 = new Graph_lib::Open_polyline;
        try
        {
            l = Func_trace(function, q, obst, pl, equation.right);
        }
        catch (...)
        {
            main_win.control_win.print_text(s);
        }

        for (auto i : l)
        {
            Point *j = new Point(i.first, i.second);
            fn2->add(*j);
        }
        fn2->set_color(Graph_lib::Color::red);
        main_win.attach(*fn2);
        break;
    }
    // leave
    case (game_state::LEAVE):
    {
        main_win.~Screen();
        break;
    }
    // respawn
    case (game_state::RESPAWN):
    {
        for (int i{0}; i < show_obs.size(); ++i)
        {
            main_win.detach(show_obs[i]);
        }
        // Удаление старых объектов
        show_obs.clear();
        main_win.redraw();
        obst.clear();

        std::cout << show_obs.size();
        GenerateObstacles(genset, obst);
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
            main_win.attach(show_obs[i]);
        };

        for (int i{0}; i < show_pl.size(); ++i)
        {
            main_win.detach(show_pl[i]);
        }
        for (size_t i = 0; i < pl.size(); ++i)
        {
            GeneratePlayer(genset, pl[i], obst, pl);
            pl[i].Revive();
        }
        show_pl.clear();

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
            main_win.attach(show_pl[i]);
        };
        for (int i{0}; i < show_nm.size(); ++i)
        {
            main_win.detach(show_nm[i]);
        }
        show_nm.clear();
        for (int i{0}; i < show_pl.size(); ++i)
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
            c->set_color(Graph_lib::Color::dark_magenta);
            show_nm.push_back(c);
            main_win.attach(show_nm[i]);
        };

        break;
    }
    }
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
