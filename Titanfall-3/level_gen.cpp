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

Point SufDistancePoint() {
    while (True)
    {
        Point TryPoint = {randint(1200), randint(720)};

        for (size_t i = 0, i < obstacles.size(); i++) {
            if (pow(pow(obstacles[i].x, 2) + pow(obstacles[i], 2), 0.5) < ShortDist)
            break;
    }
    }
    return TryPoint; // ПРОГА МОЖЕТ ЗАВИСНУТЬ, ЕСЛИ НА ПОЛЕ НЕТ МЕСТА
}