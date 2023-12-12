#include "level_gen.h"
#include "settings.h"
#include <cmath>
#include <ctime>

using namespace Graph_lib;

double dist(Point a, Point b) { return pow(pow(a.x - b.x, 2) + pow(a.y - b.y, 2), 0.5); }

// random tech
void initGenerator(PRNG &generator) {// generates seed for random
    const unsigned seed = unsigned(rand());
    generator.engine.seed(seed);// Получение случайного зерна для рандома
}

unsigned random(PRNG &generator, unsigned minValue, unsigned maxValue) {// gives random ints
    // Создаётся распределение
    std::uniform_int_distribution<unsigned> distribution(minValue, maxValue);
    // Вычисляется число, вызвав распределение как функцию, передав генератор произвольных чисел аргументом
    return distribution(generator.engine);
}

// obstacles generation
Point NotsoRandomPoint(double Power) {// Point "Power" far away from the center
    PRNG generator;
    initGenerator(generator);

    int xsign = (rand() % 2 == 0) ? 1 : -1;
    int ysign = (rand() % 2 == 0) ? 1 : -1;
    int x = static_cast<int>(pow(random(generator, 0,
                                        static_cast<int>(pow((FieldWidth - SpawnObsWallMinDist) / 2, Power))),
                                 1 / Power)) *
                    xsign +
            (FieldWidth / 2) + (SpawnObsWallMinDist / 2);
    int y = static_cast<int>(pow(random(generator, 0,
                                        static_cast<int>(pow((FieldHeight - SpawnObsWallMinDist) / 2, Power))),
                                 1 / Power)) *
                    ysign +
            (FieldHeight / 2) + (SpawnObsWallMinDist / 2);
    return Graph_lib::Point(x, y);
}

void HugeObsSpawn(std::vector<Obstacle> &Obs_vec) {// adds obstacles to HugeObstacles
    PRNG generator;
    initGenerator(generator);

    for (size_t i = 0; i < NumOfHugeObs; ++i) {
        Point Center{NotsoRandomPoint(HugeObsPower)};
        int Radius = random(generator, HugeObsMinRad, HugeObsMaxRad);

        Obstacle Obs = Obstacle{Center, Radius, false};
        Obs_vec.push_back(Obs);
    }
}

void MediumObsSpawn(std::vector<Obstacle> &Obs_vec) {// adds obstacles to MediumObstacles
    PRNG generator;
    initGenerator(generator);

    for (size_t i = 0; i < NumOfMediumObs; ++i) {
        Point Center{NotsoRandomPoint(MediumObsPower)};
        int Radius = random(generator, MediumObsMinRad, MediumObsMaxRad);

        Obstacle Obs = Obstacle{Center, Radius, false};
        Obs_vec.push_back(Obs);
    }
}

void SmallObsSpawn(std::vector<Obstacle> &Obs_vec) {// adds obstacles to SmallObstacles
    PRNG generator;
    initGenerator(generator);

    for (size_t i = 0; i < NumOfSmallObs; ++i) {
        Point Center{NotsoRandomPoint(SmallObsPower)};
        int Radius = random(generator, SmallObsMinRad, SmallObsMaxRad);

        Obstacle Obs = Obstacle{Center, Radius, false};
        Obs_vec.push_back(Obs);
    }
}

bool ObsDistPlayerCheck(const Obstacle &Obs, const std::vector<Player> &players_vec) {// obstacle not overlapping player
    for (size_t i = 0; i < players_vec.size(); ++i) {
        if (dist(Obs.center, Point{-100, -100}) < (Obs.radius + PlayerRad + SpawnObsMinDist)) {
            return false;
        }
    }
    return true;
}

void ObstaclesRespawn(std::vector<Player> &players, std::vector<Obstacle> &Obs_vec) {// regenerates obstacles with existing players
    PRNG generator;
    initGenerator(generator);

    Obs_vec.clear();

    for (size_t i = 0; i < NumOfHugeObs; ++i) {
        for (size_t j = 0; j < ObstacleReSpawnTries; ++j) {
            Point Center{NotsoRandomPoint(HugeObsPower)};
            int Radius = random(generator, HugeObsMinRad, HugeObsMaxRad);
            Obstacle Obs = Obstacle{Center, Radius, false};
            if (ObsDistPlayerCheck(Obs, players)) {
                Obs_vec.push_back(Obs);
                break;
            }
        }
        // throw NoSpaceObstacleException();
    }// Если большие препятствия нашли себе место, то остальные и подавно найдут

    for (size_t i = 0; i < NumOfMediumObs; ++i) {
        for (size_t j = 0; j < ObstacleReSpawnTries; ++j) {
            Point Center{NotsoRandomPoint(MediumObsPower)};
            int Radius = random(generator, MediumObsMinRad, MediumObsMaxRad);
            Obstacle Obs = Obstacle{Center, Radius, false};
            if (ObsDistPlayerCheck(Obs, players)) {
                Obs_vec.push_back(Obs);
                break;
            }
        }
    }

    for (size_t i = 0; i < NumOfSmallObs; ++i) {
        Point Center{NotsoRandomPoint(SmallObsPower)};
        int Radius = random(generator, SmallObsMinRad, SmallObsMaxRad);
        Obstacle Obs = Obstacle{Center, Radius, false};
        if (ObsDistPlayerCheck(Obs, players)) {
            Obs_vec.push_back(Obs);
        }
    }
}

// players generation
bool PlayerDistObsCheck(const Point &Player_pos, const std::vector<Obstacle> &Obs_vec) {// Player not overlapping Obstacles
    for (size_t i = 0; i < Obs_vec.size(); ++i) {
        if (dist(Player_pos, Obs_vec[i].center) < (Obs_vec[i].radius + PlayerRad + SpawnObsMinDist)) {
            return false;
        }
    }
    return true;
}

bool PlayerDistPlayersCheck(const Point &Player_pos, const std::vector<Player> &players_vec) {// Player not overlapping Players
    for (size_t i = 0; i < players_vec.size(); ++i) {
        if (dist(Player_pos, players_vec[i].GetPos()) < (PlayerRad + PlayerRad + SpawnBetwMinDist)) {
            return false;
        }
    }
    return true;
}

void GenerateObstacles(std::vector<Obstacle> &obstacles) {
    HugeObsSpawn(obstacles);
    MediumObsSpawn(obstacles);
    SmallObsSpawn(obstacles);
}

void GeneratePlayer(Player &player_, const std::vector<Obstacle> &obstacles,
                    const std::vector<Player> &players_vec) {
    PRNG generator;
    initGenerator(generator);

    for (size_t i = 0; i < PlayerSpawnTries; ++i) {
        Point Player = Point{static_cast<int>(random(generator, SpawnWallMinDist, FieldWidth - SpawnWallMinDist)),
                             static_cast<int>(random(generator, SpawnWallMinDist, FieldHeight - SpawnWallMinDist))};

        if ((PlayerDistObsCheck(Player, obstacles)) && (PlayerDistPlayersCheck(Player, players_vec))) {
            player_.SetPos(Player);
            return;
        }
    }
    // throw NoSpacePlayerException();
}
