#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <Graph_lib/Point.h>
#include <iostream>
#include <string>
#include <vector>

struct Obstacle
{
    Graph_lib::Point center;
    int radius;
    bool hole; // true - белое
};

class Player
{
private:
    std::string name;
    int x, y;
    bool alive;
    // if true - респавнит игрока на новом месте и alive = true
    bool awaits_respawn;

public:
    Player() = default;
    ~Player() {}
    Player(const std::string &namePlayer, int xcord, int ycord);
    Player(const std::string &namePlayer);
    std::string GetName();
    std::pair<int, int> GetCords();
    void Kill() { alive = false; }
    bool IsAlive() const { return alive; }
    void Revive();
    bool NeedResp() const { return awaits_respawn; }
    void SetPos(Graph_lib::Point point)
    {
        x = point.x;
        y = point.y;
    }

    Graph_lib::Point GetPos() const
    {
        return Graph_lib::Point{x, y};
    }
};
std::vector<std::pair<int, int>> cut_x_y(std::vector<Obstacle> &findxyobst);            // все вырезы
std::vector<std::pair<int, int>> obstac_x_y(std::vector<Obstacle> &findxyobst);         // все препятсвия
std::vector<std::pair<int, int>> player_x_y(std::vector<Graph_lib::Point> &findxyobst); // все х и у игроков
Player check_player(int x, int y, std::vector<Player> &players);                        // находим того самого игрока

std::vector<std::pair<int, int>> Func_trace(std::string &func_enter, std::pair<int, int> &cords,
                                            std::vector<Obstacle> &obstacle_mini,
                                            std::vector<Player> &players_cords, bool right_true);
// Graph_lib::Point *check_player(int x, int y, vector<Graph_lib::Point *> players);

#endif // TITANFALL_3_ENVIRONMENT_H