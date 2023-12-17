#include "level_gen.h"
#include "settings.h"
#include <cmath>
#include <ctime>

using namespace Graph_lib;

double dist(Point a, Point b) { return pow(pow(a.x - b.x, 2) + pow(a.y - b.y, 2), 0.5); }

// random tech
void initGenerator(PRNG &generator)
{ // generates seed for random
    const unsigned seed = unsigned(time(NULL));
    generator.engine.seed(seed); // Получение случайного зерна для рандома
}

unsigned random(PRNG &generator, unsigned minValue, unsigned maxValue)
{ // gives random ints
    // Создаётся распределение
    std::uniform_int_distribution<unsigned> distribution(minValue, maxValue);
    // Вычисляется число, вызвав распределение как функцию, передав генератор произвольных чисел аргументом
    return distribution(generator.engine);
}

// obstacles generation
Point NotsoRandomPoint(GenerationSettings &settings, PRNG &generator, double Power)
{ // Point "Power" far away from the center
    int xsign = (rand() % 2 == 0) ? 1 : -1;
    int ysign = (rand() % 2 == 0) ? 1 : -1;
    int x = static_cast<int>(
                pow(random(generator, 0,
                           static_cast<int>(pow((settings.FieldWidth - settings.SpawnObsWallMinDist) / 2, Power))),
                    1 / Power)) *
                xsign +
            (settings.FieldWidth / 2) + (settings.SpawnObsWallMinDist / 2);
    int y = static_cast<int>(
                pow(random(generator, 0,
                           static_cast<int>(pow((settings.FieldHeight - settings.SpawnObsWallMinDist) / 2, Power))),
                    1 / Power)) *
                ysign +
            (settings.FieldHeight / 2) + (settings.SpawnObsWallMinDist / 2);
    return Graph_lib::Point(x, y);
}

void HugeObsSpawn(GenerationSettings &settings, PRNG &generator, std::vector<Obstacle> &Obs_vec)
{ // adds obstacles to HugeObstacles
    for (int i = 0; i < settings.NumOfHugeObs; ++i)
    {
        Point Center{NotsoRandomPoint(settings, generator, settings.HugeObsPower)};
        int Radius = random(generator, settings.HugeObsMinRad, settings.HugeObsMaxRad);

        Obstacle Obs = Obstacle{Center, Radius, false};
        Obs_vec.push_back(Obs);
    }
}

void MediumObsSpawn(GenerationSettings &settings, PRNG &generator, std::vector<Obstacle> &Obs_vec)
{ // adds obstacles to MediumObstacles
    for (int i = 0; i < settings.NumOfMediumObs; ++i)
    {
        Point Center{NotsoRandomPoint(settings, generator, settings.MediumObsPower)};
        int Radius = random(generator, settings.MediumObsMinRad, settings.MediumObsMaxRad);

        Obstacle Obs = Obstacle{Center, Radius, false};
        Obs_vec.push_back(Obs);
    }
}

void SmallObsSpawn(GenerationSettings &settings, PRNG &generator, std::vector<Obstacle> &Obs_vec)
{ // adds obstacles to SmallObstacles
    for (int i = 0; i < settings.NumOfSmallObs; ++i)
    {
        Point Center{NotsoRandomPoint(settings, generator, settings.SmallObsPower)};
        int Radius = random(generator, settings.SmallObsMinRad, settings.SmallObsMaxRad);

        Obstacle Obs = Obstacle{Center, Radius, false};
        Obs_vec.push_back(Obs);
    }
}

bool ObsDistPlayerCheck(GenerationSettings &settings, const Obstacle &Obs, const std::vector<Player> &players_vec)
{ // obstacle not overlapping player
    for (size_t i = 0; i < players_vec.size(); ++i)
    {
        if (dist(Obs.center, Point{-100, -100}) < (Obs.radius + settings.PlayerRad + settings.SpawnObsMinDist))
        {
            return false;
        }
    }
    return true;
}

void ObstaclesRespawn(GenerationSettings &settings, std::vector<Player> &players,
                      std::vector<Obstacle> &Obs_vec)
{ // regenerates obstacles with existing players
    Obs_vec.clear();

    PRNG generator;
    initGenerator(generator);

    for (int i = 0; i < settings.NumOfHugeObs; ++i)
    {
        for (int j = 0; j < settings.ObstacleReSpawnTries; ++j)
        {
            Point Center{NotsoRandomPoint(settings, generator, settings.HugeObsPower)};
            int Radius = random(generator, settings.HugeObsMinRad, settings.HugeObsMaxRad);

            Obstacle Obs = Obstacle{Center, Radius, false};
            if (ObsDistPlayerCheck(settings, Obs, players))
            {
                Obs_vec.push_back(Obs);
                break;
            }
        }
        // throw NoSpaceObstacleException();
    } // Если большие препятствия нашли себе место, то остальные и подавно найдут

    for (int i = 0; i < settings.NumOfMediumObs; ++i)
    {
        for (int j = 0; j < settings.ObstacleReSpawnTries; ++j)
        {
            Point Center{NotsoRandomPoint(settings, generator, settings.MediumObsPower)};
            int Radius = random(generator, settings.MediumObsMinRad, settings.MediumObsMaxRad);
            Obstacle Obs = Obstacle{Center, Radius, false};
            if (ObsDistPlayerCheck(settings, Obs, players))
            {
                Obs_vec.push_back(Obs);
                break;
            }
        }
    }

    for (int i = 0; i < settings.NumOfSmallObs; ++i)
    {
        for (int j = 0; j < settings.ObstacleReSpawnTries; ++j)
        {
            Point Center{NotsoRandomPoint(settings, generator, settings.SmallObsPower)};
            int Radius = random(generator, settings.SmallObsMinRad, settings.SmallObsMaxRad);
            Obstacle Obs = Obstacle{Center, Radius, false};
            if (ObsDistPlayerCheck(settings, Obs, players))
            {
                Obs_vec.push_back(Obs);
                break;
            }
        }
    }
}

// players generation
bool PlayerDistObsCheck(GenerationSettings &settings, const Point &Player_pos, const std::vector<Obstacle> &Obs_vec)
{ // Player not overlapping Obstacles
    for (size_t i = 0; i < Obs_vec.size(); ++i)
    {
        if (dist(Player_pos, Obs_vec[i].center) < (Obs_vec[i].radius + settings.PlayerRad + settings.SpawnObsMinDist))
        {
            return false;
        }
    }
    return true;
}

bool PlayerDistPlayersCheck(GenerationSettings &settings, const Point &Player_pos,
                            const std::vector<Player> &players_vec)
{ // Player not overlapping Players
    for (size_t i = 0; i < players_vec.size(); ++i)
    {
        if (dist(Player_pos, players_vec[i].GetPos()) < (2 * settings.PlayerRad + settings.SpawnBetwMinDist))
        {
            return false;
        }
    }
    return true;
}

void GenerateObstacles(GenerationSettings &settings, std::vector<Obstacle> &obstacles)
{
    PRNG generator;
    initGenerator(generator);

    HugeObsSpawn(settings, generator, obstacles);
    MediumObsSpawn(settings, generator, obstacles);
    SmallObsSpawn(settings, generator, obstacles);
}

void GeneratePlayer(GenerationSettings &settings, Player &player_, const std::vector<Obstacle> &obstacles,
                    const std::vector<Player> &players_vec)
{
    PRNG generator;
    initGenerator(generator);

    for (size_t i = 0; i < settings.PlayerSpawnTries; ++i)
    {
        Point Player = Point{static_cast<int>(random(generator, settings.SpawnWallMinDist,
                                                     settings.FieldWidth - settings.SpawnWallMinDist)),
                             static_cast<int>(random(generator, settings.SpawnWallMinDist,
                                                     settings.FieldHeight - settings.SpawnWallMinDist))};

        if ((PlayerDistObsCheck(settings, Player, obstacles)) && (PlayerDistPlayersCheck(settings, Player, players_vec)))
        {
            player_.SetPos(Player);
            return;
        }
    }
    // throw NoSpacePlayerException();
}
