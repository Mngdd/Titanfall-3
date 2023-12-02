#include "level_gen.h"
#include "settings.h"
#include <cmath>
#include <ctime>
//#include "std_lib_facilities.h"


std::vector<Obstacle*> Obstacles;
std::vector<Point*> Players;

// random tech
void initGenerator(PRNG &generator)    // generates seed for random
{
    const unsigned seed = unsigned(std::time(nullptr));
    generator.engine.seed(seed);  // Получение случайного зерна для рандома
}

unsigned random(PRNG &generator, unsigned minValue, unsigned maxValue) // gives random ints
{
    // Создаётся распределение
    std::uniform_int_distribution<unsigned> distribution(minValue, maxValue);
    // Вычисляется число, вызвав распределение как функцию, передав генератор произвольных чисел аргументом
    return distribution(generator.engine);
}

// obstacles generation
Point NotsoRandomPoint(double Power)   // Point "Power" far away from the center
{
    PRNG generator;
    initGenerator(generator);
    srand(time(NULL));

    int xsign = (rand() % 2 == 0) ? 1 : -1;
    int ysign = (rand() % 2 == 0) ? 1 : -1;
    int x = static_cast<int>(pow(random(generator, SpawnObsWallMinDist, static_cast<int>(pow((FieldLength - SpawnObsWallMinDist) / 2, Power))), 1 / Power)) * xsign;
    int y = static_cast<int>(pow(random(generator, SpawnObsWallMinDist, static_cast<int>(pow((FieldWidth - SpawnObsWallMinDist) / 2, Power))), 1 / Power)) * ysign;
    return Point point{x, y};
}

void HugeObsSpawn()    // adds obstacles to HugeObstacles
{
    PRNG generator;
    initGenerator(generator);
    srand(time(NULL));

    for (size_t i = 0; i < NumOfHugeObs; ++i) {
        Point Center{NotsoRandomPoint(HugeObsPower)};
        int Radius = random(generator, HugeObsMinRad, HugeObsMaxRad);

        Obstacle *Obstacle = {Center, Radius, false};
        Obstacles.push_back(Obstacle);
    }
}

void MediumObsSpawn()  // adds obstacles to MediumObstacles
{
    PRNG generator;
    initGenerator(generator);
    srand(time(NULL));
    for (size_t i = 0; i < NumOfMediumObs; ++i) {
        Point Center{NotsoRandomPoint(MediumObsPower)};
        int Radius = random(generator, MediumObsMinRad, MediumObsMaxRad);

        Obstacle *Obstacle = {Center, Radius, false};
        Obstacles.push_back(Obstacle);
    }
}

void SmallObsSpawn()   // adds obstacles to SmallObstacles
{
    PRNG generator;
    initGenerator(generator);
    srand(time(NULL));
    Point Center{random(generator, SpawnObsWallMinDist, FieldLength),
                 random(generator, SpawnObsWallMinDist, FieldWidth)};
    int Radius = random(generator, SmallObsMinRad, SmallObsMaxRad);

    Obstacle *Obstacle = {Center, Radius, false};
    Obstacles.push_back(Obstacle);
}

bool ObsDistPlayerCheck(Obstacle const *Obstacle&)   // obstacle not overlapping player
{
    for (size_t i = 0; i < Players.size(); ++i) {
        if (dist((*Obstacle).center, Players[i]) <
            ((*Obstacle).radius + PlayerRad + SpawnObsMinDist)) {
            return true;
        } else {
            return false;
        }
    }
}

void ObstaclesRespawn() // regenerates obstacles with existing players
{
    PRNG generator;
    initGenerator(generator);
    srand(time(NULL));
    Obstacles.clear();

    for (size_t i = 0; i < NumOfHugeObs; ++i) {
        for (size_t j = 0; j < ObstacleReSpawnTries; ++j) {
            Point Center{NotsoRandomPoint(HugeObsPower)};
            int Radius = random(generator, HugeObsMinRad, HugeObsMaxRad);
            Obstacle *Obstacle = {Center, Radius};
            if (ObsDistPlayerCheck(Obstacle)) {
                Obstacles.push_back(Obstacle);
            }
        }
        error("Not enough space for Huge Obstacle! ")
    } // Если большие препятствия нашли себе место, то остальные и подавно найдут

    for (size_t i = 0; i < NumOfMediumObs; ++i) {
        Point Center{NotsoRandomPoint(MediumObsPower)};
        int Radius = random(generator, MediumObsMinRad, MediumObsMaxRad);
        Obstacle *Obstacle = {Center, Radius};
        if (ObsDistPlayerCheck(Obstacle)) {
            Obstacles.push_back(Obstacle);
        }
    }

    for (size_t i = 0; i < NumOfSmallObs; ++i) {
        Point Center{NotsoRandomPoint(SmallObsPower)};
        int Radius = random(generator, SmallObsMinRad, SmallObsMaxRad);
        Obstacle *Obstacle = {Center, Radius};
        if (ObsDistPlayerCheck(Obstacle)) {
            Obstacles.push_back(Obstacle);
        }
    }

}

// players generation
bool PlayerDistObsCheck(Point const Player&)   // Player not overlapping Obstacles
{
    for (size_t i = 0; i < Obstacles.size(); ++i) {
        if (dist(Player, (*Obstacles[i]).center) <
            ((*Obstacles[i]).radius + PlayerRad + SpawnObsMinDist)) {
            return true;
        } else {
            return false;
        }
    }
}

bool PlayerDistPlayersCheck(Point const Player&)   // Player not overlapping Players
{
    for (size_t i = 0; i < Players.size(); ++i) {
        if (dist(Player, Players[i]) < (PlayerRad + PlayerRad + SpawnBetwMinDist)) {
            return true;
        } else {
            return false;
        }
    }

}

void PlayerSpawn()      // adds player to Players
{
    PRNG generator;
    initGenerator(generator);
    srand(time(NULL));
    for (size_t i = 0, i<PlayerSpawnTries, ++i) {
        Point *Player = new Point{random(generator, SpawnWallMinDist, FieldLength - SpawnWallMinDist),
                                random(generator, SpawnWallMinDist, FieldWidth - SpawnWallMinDist)};

        if ((PlayerDistObsCheck(Center)) && (PlayerDistPlayersCheck(Center)))
        {
            Players.push_back(Player);
            return;
        }
    }
    error("Not enough space for player! ")
}

void PlayersSpawn()    // adds players to Players
{
    for (size_t i = 0; i < NumOfPlayers; ++i) {
        PlayerSpawn();
    }
}

void Generate() {
    HugeObsSpawn();
    MediumObsSpawn();
    SmallObsSpawn();
    PlayersSpawn();
}
