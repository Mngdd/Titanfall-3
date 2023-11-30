#include "level_gen.h"
#include "settings.h"
#include <cmath>
#include <ctime>
#include <random>

vector<Circle*> HugeObstacles;
vector<Circle*> MediumObstacles;
vector<Circle*> SmallObstacles;

vector<Circle*> Obstacles;
vector<Circle*> Players;

struct PRNG
{
    std::mt19937 engine;
};

void initGenerator (PRNG& generator)
{
    const unsigned seed = unsigned(std::time(nullptr));
    generator.engine.seed(seed);  // Получение случайного зерна для рандома
}

unsigned random (PRNG& generator, unsigned minValue, unsigned maxValue)
{
    // Создаётся распределение
    std::uniform_int_distribution<unsigned> distribution(minValue, maxValue);
    // Вычисляется число, вызвав распределение как функцию, передав генератор произвольных чисел аргументом
    return distribution(generator.engine);
}

void HugeObsSpawn ()
{  // adds obstacles to HugeObstacles
    for (size_t i = 0; i < NumOfHugeObs; ++i)
    {
        PRNG generator;
        initGenerator(generator);
        srand(time(NULL));

        char xxsign = rand() % 2;
        char yysign = rand() % 2;
        int xsign = (xxsign == 0) ? 1 : -1;
        int ysign = (yysign == 0) ? 1 : -1;
        int x = static_cast<int>(
                    pow(random(generator, 0, static_cast<int>(pow(FieldLength / 2, HugeObsPower))), 1 / HugeObsPower)) *
                xsign;
        int y = static_cast<int>(
                    pow(random(generator, 0, static_cast<int>(pow(FieldWidth / 2, HugeObsPower))), 1 / HugeObsPower)) *
                ysign;

        Point Center{x, y};
        int Radius = random(generator, HugeObsMinRad, HugeObsMaxRad);

        Circle* Obstacle = new Circle{Center, Radius};
        (*Obstacle).set_fill_color(Color::black);
        HugeObstacles.push_back(Obstacle);
    }
}

void MediumObsSpawn ()
{  // adds obstacles to MediumObstacles
    for (size_t i = 0; i < NumOfMediumObs; ++i)
    {
        PRNG generator;
        initGenerator(generator);
        srand(time(NULL));

        char xxsign = rand() % 2;
        char yysign = rand() % 2;
        int xsign = (xxsign == 0) ? 1 : -1;
        int ysign = (yysign == 0) ? 1 : -1;
        int x = static_cast<int>(pow(random(generator, 0, static_cast<int>(pow(FieldLength / 2, MediumObsPower))),
                                     1 / MediumObsPower)) *
                xsign;
        int y = static_cast<int>(pow(random(generator, 0, static_cast<int>(pow(FieldWidth / 2, MediumObsPower))),
                                     1 / MediumObsPower)) *
                ysign;

        Point Center{x, y};
        int Radius = random(generator, MediumObsMinRad, MediumObsMaxRad);

        Circle* Obstacle = new Circle{Center, Radius};
        (*Obstacle).set_fill_color(MediumObsColor);
        MediumObstacles.push_back(Obstacle);
    }
}

void SmallObsSpawn ()
{  // adds obstacles to SmallObstacles
    PRNG generator;
    initGenerator(generator);
    srand(time(NULL));

    int x = random(generator, 0, FieldLength);
    int y = random(generator, 0, FieldWidth);

    Point Center{x, y};
    int Radius = random(generator, SmallObsMinRad, SmallObsMaxRad);

    Circle* Obstacle = new Circle{Center, Radius};
    (*Obstacle).set_fill_color(SmallObsColor);
    SmallObstacles.push_back(Obstacle);
}

void PlayersSpawn ()
{  // adds players to Players
    for (size_t j = 0; j < NumOfPlayers; ++j)
    {
        while (true)
        {
            Point Center{Graph_lib::randint(SpawnWallMinDist, FieldLength - SpawnWallMinDist),
                         Graph_lib::randint(SpawnWallMinDist, FieldWidth - SpawnWallMinDist)};
            bool Checks = true;

            // CHECKS:
            for (size_t i = 0; i < HugeObstacles.size(); ++i)
            {
                if (dist(Center, (*HugeObstacles[i]).center()) <
                    (*HugeObstacles[i]).radius() + PlayerRad + SpawnObsMinDist)
                {
                    Checks = false;
                    break;
                }
            }

            for (size_t i = 0; i < MediumObstacles.size(); ++i)
            {
                if (dist(Center, (*MediumObstacles[i]).center()) <
                    (*MediumObstacles[i]).radius() + PlayerRad + SpawnObsMinDist)
                {
                    Checks = false;
                    break;
                }
            }

            for (size_t i = 0; i < SmallObstacles.size(); ++i)
            {
                if (dist(Center, (*SmallObstacles[i]).center()) <
                    (*SmallObstacles[i]).radius() + PlayerRad + SpawnObsMinDist)
                {
                    Checks = false;
                    break;
                }
            }

            for (size_t i = 0; i < Players.size(); ++i)
            {
                if (dist(Center, (*Players[i]).center()) < PlayerRad + PlayerRad + SpawnBetwMinDist)
                {
                    Checks = false;
                    break;
                }
            }

            if (Checks)
            {
                Circle* Player = new Circle{Center, PlayerRad};
                (*Player).set_fill_color(Color::dark_green);
                Players.push_back(Player);
                break;
            }
        }
    }
}

void Generate ()
{
    HugeObsSpawn();
    MediumObsSpawn();
    SmallObsSpawn();
    PlayersSpawn();
    for (Circle* Obstacle : HugeObstacles)
    {
        Obstacles.push_back(Obstacle);
    }
    for (Circle* Obstacle : MediumObstacles)
    {
        Obstacles.push_back(Obstacle);
    }
    for (Circle* Obstacle : SmallObstacles)
    {
        Obstacles.push_back(Obstacle);
    }
}
