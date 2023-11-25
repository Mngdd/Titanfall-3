#include <cmath>
#include <Graph_lib/Point.h>
#include <Graph_lib/Graph.h>

#include "level_gen.h"
#include "std_lib_facilities.h"

vector<Circle> obstacles;
vector<Point> players;

Point PlayerSpawnPoint() { // returns Point for player to spawn
    while true
    {
        Point TryPoint = {randint(1200), randint(720)};
        bool fit = true;

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

Point PlayerRespawnPoint() { // returns Point for player to respawn
    while true
    {
        Point TryPoint = {randint(1200), randint(720)};
        bool fit = true;

        for (size_t i = 0, i < obstacles.size(); i++) {
            if (dist(obstacles[i].center(), TryPoint) < ShortDist) {
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


