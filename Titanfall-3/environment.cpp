//
// Created by khris on 22.11.2023.
//
#include "environment.h"
#include "settings.h"
#include <Graph_lib/Point.h>
#include <bits/stdc++.h>
#include "parser.h"
using namespace std;

Player::Player(const string &namePlayer, int xcord, int ycord)
{
    name = namePlayer;
    x = xcord;
    y = ycord;
    alive = true;
    awaits_respawn = true;
    // body = new  = new Graph_lib::Circle{Point(x, y), PlayerRad};
}

Player::Player(const string &namePlayer)
{
    name = namePlayer;
    x = y = -1;
    alive = true;
    awaits_respawn = true;
    // body = new  = new Graph_lib::Circle{Point(x, y), PlayerRad};
}

string Player::GetName()
{
    return name;
}

void Player::Revive()
{
    alive = true;
    awaits_respawn = false;
}

pair<int, int> Player::GetCords()
{
    return pair<int, int>{x, y};
}

///  мат выражения
//
// It supports operators: + - * / ^ ( )

// It supports math functions : SIN, COS, TAN, ASIN, ACOS, ATAN, SINH,
// COSH, TANH, ASINH, ACOSH, ATANH, LN, LOG, EXP, SQRT, SQR, ROUND, INT.

// It supports variables A to Z.

std::vector<std::pair<int, int>> cut_x_y(vector<Obstacle> &findxyobst)
{
    std::vector<std::pair<int, int>> points;
    for (const auto &i : findxyobst)
    {
        if (i.hole == true)
        {
            int a = i.center.x;
            int b = i.center.y;
            int r = i.radius;
            for (int x = a - r; x <= a + r; x += 1)
            {
                for (int y = b - r; y <= b + r; y += 1)
                {
                    if (std::pow(x - a, 2) + std::pow(y - b, 2) <= std::pow(r, 2))
                    {
                        points.push_back(std::make_pair(x, y));
                    }
                }
            }
        }
    }
    return points;
}

std::vector<std::pair<int, int>> obstac_x_y(vector<Obstacle> &findxyobst)
{
    std::vector<std::pair<int, int>> points;

    for (size_t i = 0; i < findxyobst.size(); ++i)
    {
        if (findxyobst[i].hole == false)
        {
            int a = findxyobst[i].center.x;
            int b = findxyobst[i].center.y;
            int r = findxyobst[i].radius;
            for (int x = a - r; x <= a + r; x += 1)
            {
                for (int y = b - r; y <= b + r; y += 1)
                {
                    if (std::pow(x - a, 2) + std::pow(y - b, 2) <= std::pow(r, 2))
                    {
                        points.push_back(std::make_pair(x, y));
                    }
                }
            }
        }
    }

    return points;
}

vector<pair<int, int>> player_x_y(vector<Player> &findxyobst)
{
    vector<pair<int, int>> points;
    for (const auto &i : findxyobst)
    {

        int a = i.GetPos().x;
        int b = i.GetPos().y;
        const int r = PlayerRad;
        for (int x = a - r; x <= a + r; x += 1)
        {
            for (int y = b - r; y <= b + r; y += 1)
            {
                if (std::pow(x - a, 2) + std::pow(y - b, 2) <= std::pow(r, 2))
                {
                    points.push_back(std::make_pair(x, y));
                }
            }
        }
    }

    return points;
}

Player check_player(int x, int y, vector<Player> &players)
{ // тут ищем кому приндлежит данная точка
    int min_dist = PlayerRad + 2;
    Player closest;
    for (const auto &i : players)
    {
        int x_player = i.GetPos().x;
        int y_player = i.GetPos().y;
        double distance = sqrt(pow(x - x_player, 2) + sqrt(pow(y - y_player, 2)));
        if (distance < min_dist)
        {
            min_dist = distance;
            closest = i;
        }
    }
    return closest;
}

vector<pair<int, int>> Func_trace(string &func_enter, pair<int, int> &cords,
                                  vector<Obstacle> &obstacle_mini,
                                  vector<Player> &players_cords, bool right_true)
{
    // const int size_map_y = FieldHeight; // длина поля для пробега y
    const int size_map_x = FieldWidth; // длина поля для пробега х

    // bool right_true = true; // в какую сторону стрелять
    size_t pos = func_enter.find('x');
    vector<pair<int, int>> cord_vector;
    vector<pair<int, int>> white_cords = cut_x_y(obstacle_mini);     // нашли все координаты вырезов
    vector<pair<int, int>> black_cords = obstac_x_y(obstacle_mini);  // нашли все координаты препятствия
    vector<pair<int, int>> gamers_cords = player_x_y(players_cords); // нашли все координаты игроков

    if (pos != string::npos) // проверка есть ли аргумент Х в функции
    {
        vector<int> index;
        for (size_t i = 0; i <= func_enter.size() - 1; ++i)
        {
            if (func_enter[i] == 'x')
            {
                index.push_back(i);
            }
        }
        for (int x = 0; x <= size_map_x; ++x) // перебираем х
        {
            string str_imagin = func_enter;
            string new_value = to_string(x); // из инта в стринг
            for (size_t i = 0; i <= index.size() - 1; ++i)
            {
                size_t pos1 = str_imagin.find('x');
                str_imagin.replace(pos1, 1, new_value);
            }
            char expstr[256];
            strcpy(expstr, str_imagin.c_str()); //
            parser ob;
            double ans = ob.eval_exp(expstr); // вычисляем мат выражение
            if (*ob.errormsg)
            {
                throw ob.errormsg;
            }

            else
            {
                int x_i;
                if (right_true) // если стреляем вправо
                {
                    x_i = cords.first + x;
                }
                else
                {
                    x_i = cords.first - x;
                }
                if (not(isnan(ans))) // значение non
                {
                    // int y_i = cords.second + static_cast<int>(round(ans));
                    int y_i = cords.second - static_cast<int>(round(ans));
                    pair<int, int> p = make_pair(x_i, y_i);
                    auto obst_for = find(black_cords.begin(), black_cords.end(), p); // для проверки на попадание в препятсвие
                    auto gamer_for = find(gamers_cords.begin(), gamers_cords.end(), p);

                    if (x <= PlayerRad + SpawnObsMinDist)
                    {
                        cord_vector.push_back(make_pair(x_i, y_i));
                        continue;
                    }
                    if (gamer_for != gamers_cords.end()) // true если попали в игрока
                    {                                    // удалям координаты из вектора
                        cord_vector.push_back(make_pair(x_i, y_i));

                        check_player(x_i, y_i, players_cords).Kill();
                        continue;
                    }

                    auto cut_for = find(white_cords.begin(), white_cords.end(), p);
                    if (cut_for != white_cords.end())
                    {
                        cord_vector.push_back(make_pair(x_i, y_i));
                        continue;
                    }

                    if (obst_for != black_cords.end())
                    { // попали в препятсвия
                        int rad = WhiteObsRad;
                        Obstacle with_out = Obstacle{Graph_lib::Point(x_i, y_i), rad, true};
                        obstacle_mini.push_back(with_out); // попали в препятствие x_i добавляем вырез
                        break;
                    }
                    cord_vector.push_back(make_pair(x_i, y_i));
                }
            }
        }
    }
    else
    {
        for (int x = 0; size_map_x >= x; ++x)
        {
            char expstr[256];
            strcpy(expstr, func_enter.c_str());
            parser ob;
            cout << expstr << endl;
            double ans = ob.eval_exp(expstr);

            if (right_true)
            {
                int x_i = cords.first + x;
                int y_i = ans;
                pair<int, int> p = make_pair(x_i, y_i);
                auto obst_for = find(black_cords.begin(), black_cords.end(), p); // для проверки на попадание в препятсвие
                auto gamer_for = find(gamers_cords.begin(), gamers_cords.end(), p);
                if (x <= PlayerRad + SpawnObsMinDist)
                {
                    continue;
                }
                if (gamer_for != gamers_cords.end()) // true если попали в игрока
                {
                    // удалям координаты из вектора
                    cord_vector.push_back(make_pair(x_i, y_i));
                    check_player(x_i, y_i, players_cords).Kill();
                    continue;
                }

                auto cut_for = find(white_cords.begin(), white_cords.end(), p);
                if (cut_for != white_cords.end())
                {
                    cord_vector.push_back(make_pair(x_i, y_i));
                    continue;
                }

                if (obst_for != black_cords.end())
                { // попали в препятсвия
                    int rad = WhiteObsRad;
                    Obstacle with_out = Obstacle{Graph_lib::Point(x_i, y_i), rad, true};
                    obstacle_mini.push_back(with_out); // попали в препятствие x_i добавляем вырез
                    break;
                }
                cord_vector.push_back(make_pair(x_i, y_i));
            }
            else
            {
                int x_i = cords.first - x;
                int y_i = ans;
                pair<int, int> p = make_pair(x_i, y_i);
                auto obst_for = find(black_cords.begin(), black_cords.end(), p); // для проверки на попадание в препятсвие
                auto gamer_for = find(gamers_cords.begin(), gamers_cords.end(), p);
                if (x <= PlayerRad + SpawnObsMinDist)
                {
                    cord_vector.push_back(make_pair(x_i, y_i));
                    continue;
                }
                if (gamer_for != gamers_cords.end()) // true если попали в игрока
                {
                    cord_vector.push_back(make_pair(x_i, y_i));
                    // удалям координаты из вектора
                    check_player(x_i, y_i, players_cords).Kill();
                    continue;
                }

                auto cut_for = find(white_cords.begin(), white_cords.end(), p);
                if (cut_for != white_cords.end())
                {
                    cord_vector.push_back(make_pair(x_i, y_i));
                    continue;
                }

                if (obst_for != black_cords.end())
                { // попали в препятсвия
                    int rad = WhiteObsRad;
                    Obstacle with_out = Obstacle{Graph_lib::Point(x_i, y_i), rad, true};
                    obstacle_mini.push_back(with_out); // попали в препятствие x_i добавляем вырез
                    break;
                }
                cord_vector.push_back(make_pair(x_i, y_i));
            }
        }
    }
    return cord_vector;
}
// for (const auto &p: cord_vector)
//{
//	cout << p.first << ' ' << p.second<< endl;
// }

///} //возможно скобку где то не закрыл....
