#ifndef TITANFALL_3_DRAW_H
#define TITANFALL_3_DRAW_H
#include "environment.h"
#include "graphics.h"
#include <Graph_lib/Point.h>
#include <vector>

// получаем точки нашей функции
std::vector<std::pair<int, int>> Func_trace(std::string func_enter, std::pair<int, int> &cords,
                                            std::vector<Obstacle *> obstacle_mini,
                                            std::vector<Player *> players_cords, bool right_true);

// отрисовывает игровое поле
void game_draw(Screen &main_win, std::vector<Player> &pl,
               std::vector<Obstacle> &obstacles, input_data equation, Player real_player);

#endif // TITANFALL_3_SERVER_H
