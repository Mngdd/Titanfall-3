#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <Graph_lib/Point.h>

#include <bits/stdc++.h>

struct Obstacle {
    Graph_lib::Point center;
    int radius;
    bool hole;//true - белое
};

class Player {
private:
    std::string name;
    int x, y;
    bool alive;

public:
    Player() = default;
    ~Player() {}
    Player(const std::string &namePlayer, int xcord, int ycord);
    Player(std::string &namePlayer, int xcord, int ycord);
    std::string GetName();
    std::pair<int, int> GetCords();
    void Kill() {alive = false;}
    bool IsAlive() {return alive;}
};

std::vector<std::pair<int, int>> Func_trace(std::string& func_enter, std::pair<int, int> &cords,
                                       std::vector<Obstacle>& obstacle_mini,
                                       std::vector<Graph_lib::Point>& players_cords);
//Graph_lib::Point *check_player(int x, int y, vector<Graph_lib::Point *> players);

#endif// TITANFALL_3_ENVIRONMENT_H