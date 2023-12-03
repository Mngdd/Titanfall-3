#ifndef ENVIROMENT_H
#define ENVIROMENT_H
#include <Graph_lib/Point.h>

#include <bits/stdc++.h>

using namespace Graph_lib;
struct Obstacle
{
    Point center;
    int radius;
    bool hole;
};

class Player
{
private:
    std::string name;
    double health;
    int x, y;

public:
    Player(std::string namePlayer, double health_point, int xcord, int ycord);
    double GetHealth();
    std::string GetName();
    std::pair<int, int> GetCords();
    void SetDamage(double damagevalue);
};

#endif // TITANFALL_3_ENVIRONMENT_H
