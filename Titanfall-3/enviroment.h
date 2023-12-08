#ifndef ENVIROMENT_H
#define ENVIROMENT_H
#include <Graph_lib/Point.h>

#include <bits/stdc++.h>

using namespace Graph_lib;
struct Obstacle
{
    Point center;
    int radius;
    bool hole; //true - белое
};

class Player
{
private:
    std::string name;
    int x, y;


public:
    Player(std::string namePlayer, int xcord, int ycord);
    std::string GetName();
    std::pair<int, int> GetCords();
};

#endif // TITANFALL_3_ENVIRONMENT_H