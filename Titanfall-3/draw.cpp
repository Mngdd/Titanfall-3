#include <FL/Enumerations.H>
#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>

#include "draw.h"
#include "level_gen.h"
#include "settings.h"

using namespace Graph_lib;
Graph_lib::Open_polyline* fn2;  // наш график
Vector_ref<Circle> show_obs;    // препятствия
Vector_ref<Circle> show_pl;     // игроки
Vector_ref<Text> show_nm;       // имена игроков

void game_draw (Screen& win, std::vector<Player>& pl, std::vector<Obstacle>& obst, input_data equation,
                Player real_player)
{  // TODO: add icon
    // очищаем вывод
    std::string s1 = "";

    win.control_win.print_text(s1);
    // координаты, того стреляет
    std::pair<int, int> q = real_player.GetCords();
    // сообщение об ошибке
    std::string s = "ERROR";
    // вектор точек графика
    std::vector<std::pair<int, int>> l;
    // получаем настройки для игры
    GenerationSettings genset = win.settings();
    // получаем строку нашей функции
    std::string function = equation.equation;
    switch (equation.g_s)
    {
    // fire
    case (game_state::FIRE):
    {
        win.detach(*fn2);
        delete fn2;
        fn2 = new Graph_lib::Open_polyline;
        try
        {
            l = Func_trace(function, q, obst, pl, equation.right);
        }
        catch (...)
        {
            win.control_win.print_text(s);
        }

        for (auto i : l)
        {
            Point* j = new Point(i.first, i.second);
            fn2->add(*j);
        }
        fn2->set_color(Graph_lib::Color::red);
        win.attach(*fn2);
        win.clear();
        win.redraw();
        break;
    }
    // leave
    case (game_state::LEAVE):
    {
        win.control_win.~InputMenu();
        win.~Screen();
        break;
    }
    // respawn
    case (game_state::RESPAWN):
    {
        respawn(win, obst, real_player, pl);
        break;
    }
    }
    // создаем препятствия и вырезы
    for (size_t i = 0; i < obst.size(); ++i)
    {
        Circle* c = new Circle{obst[i].center, obst[i].radius};
        if (obst[i].hole)
        {
            c->set_fill_color(FL_LIGHT1);
            c->set_color(FL_LIGHT1);
        }
        else
            c->set_fill_color(HugeObsColor);
        show_obs.push_back(c);
    };
    // рисуем препятствия
    for (size_t i = 0; i < show_obs.size(); ++i)
    {
        if (!obst[i].hole)
        {
            win.attach(show_obs[i]);
        }
    }
    // рисуем вырезы
    for (size_t i = 0; i < show_obs.size(); ++i)
    {
        if (obst[i].hole)
        {
            win.attach(show_obs[i]);
        }
    }
    // рисуем игроков
    for (size_t i{0}; i < pl.size(); ++i)
    {
        Circle* c = new Circle{Point(pl[i].GetCords().first, pl[i].GetCords().second), PlayerRad};
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
            std::cout << "asda";
            show_pl[i].set_fill_color(Graph_lib::Color::red);
        }
        show_pl.push_back(c);
        win.attach(show_pl[i]);
        Text* t{nullptr};
        if (pl[i].GetCords().second + PlayerRad >= FieldHeight)
        {
            t = new Text{Point(pl[i].GetCords().first, pl[i].GetCords().second + PlayerRad), pl[i].GetName()};
        }
        else
        {
            t = new Text{Point(pl[i].GetCords().first, pl[i].GetCords().second - PlayerRad), pl[i].GetName()};
        }
        t->set_font_size(NickSize);
        t->set_color(Graph_lib::Color::dark_magenta);
        show_nm.push_back(t);
        win.attach(show_nm[i]);
    };
}

// перерисовка
void respawn (Screen& win, std::vector<Obstacle>& obst, Player& player, std::vector<Player>& pl_)
{
    win.detach(*fn2);
    // получаем нужные настройки
    GenerationSettings genset = win.settings();
    // отвязываем от окна
    for (int i{0}; i < show_obs.size(); ++i)
    {
        win.detach(show_obs[i]);
    }
    // Удаление старых объектов
    show_obs.clear();
    win.redraw();
    obst.clear();
    // генерируем препятствия заново
    GenerateObstacles(genset, obst);
    for (size_t i = 0; i < obst.size(); ++i)
    {
        Circle* c = new Circle{obst[i].center, obst[i].radius};
        if (obst[i].hole)
        {
            c->set_fill_color(FL_LIGHT1);
            c->set_color(FL_LIGHT1);
        }
        else
            c->set_fill_color(HugeObsColor);

        show_obs.push_back(c);
        win.attach(show_obs[i]);
    };
    // генерируем заново игроков
    for (size_t i = 0; i < pl_.size(); ++i)
    {
        GeneratePlayer(genset, pl_[i], obst, pl_);
        pl_[i].Revive();
    }
    // отвязываем старых игроков
    for (int i{0}; i < show_pl.size(); ++i)
    {
        win.detach(show_pl[i]);
    }
    show_pl.clear();
    // рисуем новых игроков
    for (size_t i{0}; i < pl_.size(); ++i)
    {
        Circle* c = new Circle{Point(pl_[i].GetCords().first, pl_[i].GetCords().second), PlayerRad};
        if (pl_[i].IsAlive())
        {
            if (pl_[i].GetName() == player.GetName())
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
            std::cout << "dead";
            c->set_fill_color(Graph_lib::Color::dark_red);
        }
        show_pl.push_back(c);
        win.attach(show_pl[i]);
    };
    // отвязываем и удаляем старые имена
    for (int i{0}; i < show_nm.size(); ++i)
    {
        win.detach(show_nm[i]);
    }
    show_nm.clear();
    // печатаем новые имена
    for (int i{0}; i < show_pl.size(); ++i)
    {
        Text* c{nullptr};
        if (pl_[i].GetCords().second + PlayerRad >= FieldHeight)
        {
            c = new Text{Point(pl_[i].GetCords().first, pl_[i].GetCords().second + PlayerRad), pl_[i].GetName()};
        }
        else
        {
            c = new Text{Point(pl_[i].GetCords().first, pl_[i].GetCords().second - PlayerRad), pl_[i].GetName()};
        }
        c->set_font_size(NickSize);
        c->set_color(Graph_lib::Color::dark_magenta);
        show_nm.push_back(c);
        win.attach(show_nm[i]);
    };
}

void func_draw (Screen& win, std::vector<Point>& f)
{  // TODO: add icon
    Graph_lib::Open_polyline fn2;
    // create & draw function
    for (auto i : f)
    {
        Point* j = new Point(i.x, i.y);
        fn2.add(*j);
    }
    fn2.set_color(Graph_lib::Color::red);
    win.attach(fn2);
}
