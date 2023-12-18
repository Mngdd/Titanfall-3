#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <Graph_lib/Point.h>
#include <iostream>
#include <string>
#include <vector>

struct Obstacle {
    Graph_lib::Point center;
    int radius;
    bool hole;// true - белое
    Obstacle(Graph_lib::Point center_, int r_, bool h_) : center{center_}, radius{r_}, hole{h_} {}
};

class Player {
private:
    std::string name;
    int x, y;
    bool alive;
    // if true - респавнит игрока на новом месте и alive = true

public:
    std::string func;
    bool awaits_respawn; // а игрок хочет респауница или мертвым сидит
    bool right;// без геттеров и сеттеров потому что юзлес
    // однако у других оставил - иначе коду конец...
    Player() = default;
    ~Player() {}
    Player(const std::string &namePlayer, int xcord, int ycord);
    Player(const std::string &namePlayer);
    std::string GetName() const;
    void SetName(const std::string &name_) { name = name_; }
    std::pair<int, int> GetCords() const; //fixme: уже есть GetPos, нес мешно
    void Kill() { alive = false; }
    void Revive();
    bool IsAlive() const { return alive; }
    bool NeedResp() const { return awaits_respawn; }
    void SetPos(Graph_lib::Point point) {
        x = point.x;
        y = point.y;
    }
    Graph_lib::Point GetPos() const { return Graph_lib::Point{x, y}; }
};

std::vector<std::pair<int, int>> cut_x_y(std::vector<Obstacle> &findxyobst);           // все вырезы
std::vector<std::pair<int, int>> obstac_x_y(std::vector<Obstacle> &findxyobst);        // все препятсвия
std::vector<std::pair<int, int>> player_x_y(std::vector<Graph_lib::Point> &findxyobst);// все х и у игроков
Player &check_player(int x, int y, std::vector<Player> &players);                      // находим того самого игрока

std::vector<std::pair<int, int>> Func_trace(std::string &func_enter, std::pair<int, int> &cords,
                                            std::vector<Obstacle> &obstacle_mini,
                                            std::vector<Player> &players_cords, bool right_true);
// Graph_lib::Point *check_player(int x, int y, vector<Graph_lib::Point *> players);

#endif// TITANFALL_3_ENVIRONMENT_H