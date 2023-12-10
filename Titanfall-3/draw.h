#ifndef TITANFALL_3_DRAW_H
#define TITANFALL_3_DRAW_H
#include "environment.h"
#include "graphics.h"
#include <Graph_lib/Point.h>
#include <vector>

std::vector<std::pair<int, int>> Func_trace(std::string func_enter, std::pair<int, int> &cords,
                                            std::vector<Obstacle *> obstacle_mini,
                                            std::vector<Graph_lib::Point *> players_cords);

// отрисовывает игровое поле
void game_draw(MainMenu& main_win, std::vector<Player>& pl);

#endif//TITANFALL_3_SERVER_H
