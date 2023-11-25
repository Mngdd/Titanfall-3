//
// Created by khris on 22.11.2023.
//

// Термины:
// Карта будет скорее всего 1200:720
// Достаточное расстояние - примерно среднее расстояние как длина большого пальца
// Большое расстояние - по крайней мере четверть карты
// Маленькое расстояние
//
// При смерти игрок появляется на достаточном расстоянии от союзников

#include <cmath>
#include <Graph_lib/Point.h>

#include "level_gen.h"
#include "std_lib_facilities.h"

vector<Point> obstacles;
vector<Point> players;

Point PlayerSpawnPoint() {
    while true
    {
        Point TryPoint = {randint(1200), randint(720)};
        bool fit = true;

        for (size_t i = 0, i < obstacles.size(); i++) {
            if (dist(obstacles[i], TryPoint) < ShortDist) {
                fit = false;
                break;
            }
        }

        for (size_t i = 0, i < players.size(), i++) {
            if (dist(players[i], TryPoint) < ShortDist) {
                fit = false
                break;
            }
        }

        if (fit) {
            return TryPoint;
        } // ПРОГА МОЖЕТ ЗАВИСНУТЬ, ЕСЛИ НА ПОЛЕ НЕТ МЕСТА
    }
}
Point SufDistancePoint() {
    while (true)
    {
        Point TryPoint = {randint(1200), randint(720)};
        bool fit = true

        for (size_t i = 0, i < obstacles.size(); i++) {
            if (pow(pow(obstacles[i].x - TryPoint.x, 2) + pow(obstacles[i].y - TryPoint.y, 2), 0.5) < ShortDist)
            fit = false;
            break;
        }

        if (fit) {
            return TryPoint;
        } // ПРОГА МОЖЕТ ЗАВИСНУТЬ, ЕСЛИ НА ПОЛЕ НЕТ МЕСТА
    }
}