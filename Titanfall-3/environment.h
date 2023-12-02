<<<<<<< Updated upstream:Titanfall-3/environment.h
//
// Created by khris on 22.11.2023.
//
#ifndef TITANFALL_3_ENVIRONMENT_H
#define TITANFALL_3_ENVIRONMENT_H
=======
#include <Graph_lib/Point.h>

using namespace Graph_lib;

struct Obstacle {
    Point center;
    int radius;
}
>>>>>>> Stashed changes:Titanfall-3/enviroment.h

int GetCords();

double GetHealth();

void SetDamage(double damagevalue);

#endif //TITANFALL_3_ENVIRONMENT_H
