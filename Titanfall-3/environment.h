#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
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
    bool alive;

public:
    Player() = default;
    ~Player(){}
    Player(const std::string& namePlayer, int xcord, int ycord);
    Player(std::string& namePlayer, int xcord, int ycord);
    std::string GetName();
    std::pair<int, int> GetCords();
};

#endif // TITANFALL_3_ENVIRONMENT_H