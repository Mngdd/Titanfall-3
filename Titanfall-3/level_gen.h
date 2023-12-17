#ifndef LEVEL_GEN_H
#define LEVEL_GEN_H

#include "environment.h"
#include <Graph_lib/Graph.h>
#include <Graph_lib/Point.h>
#include <random>
#include <vector>

// НАСТРОЙКИ ГЕНЕРАЦИИ
struct GenerationSettings
{
    // Количество игроков для которых надо сгенерировать
    int NumOfPlayers = 6;

    int PlayerRad = 10;
    // ДЕФОЛТНЫЕ НАСТРОЙКИ
    // Ширина поля
    const int FieldWidth = 1200;
    // Высота поля
    const int FieldHeight = 720;

    // Степень ухода больших препятствий от центра
    const double HugeObsPower = 2;
    // Степень ухода средних препятствий от центра
    const double MediumObsPower = 0.5;
    // Степень ухода маленьких препятствий от центра
    const double SmallObsPower = 1;

    // Минимальное расстояние между игроками (мультиплеер)
    const int SpawnBetwMinDist = 70;
    // Минимальное расстояние от игрока до стены (больше радиуса игрока)
    const int SpawnWallMinDist = 30;
    // Минимальное расстояние от игрока до препятствия
    const int SpawnObsMinDist = 5;

    // Минимальное расстояние от препятствия до стены
    const int SpawnObsWallMinDist = 3;

    // Количество препятствий каждого вида
    int NumOfHugeObs = 5;
    int NumOfMediumObs = 5;
    int NumOfSmallObs = 5;

    // Радиусы препятствий разных размеров
    const int HugeObsMaxRad = 100;
    const int HugeObsMinRad = 65;
    const int MediumObsMaxRad = 65;
    const int MediumObsMinRad = 30;
    const int SmallObsMaxRad = 30;
    const int SmallObsMinRad = 15;

    // Количество попыток заспавнить игрока в норм месте
    const int PlayerSpawnTries = 500;
    // Количество попыток зареспавнить препятствие в норм месте
    const int ObstacleReSpawnTries = 100;
};

// random tech
struct PRNG
{
    std::mt19937 engine;
};

double dist (Graph_lib::Point a, Graph_lib::Point b);  // returns dist between two points

void initGenerator (PRNG& generator);                                     // generates seed for random
unsigned random (PRNG& generator, unsigned minValue, unsigned maxValue);  // gives random ints

Graph_lib::Point NotsoRandomPoint (GenerationSettings& settings, PRNG& generator,
                                   double Power);  // Point "Power" far away from the center

// obstacles generation
void HugeObsSpawn (GenerationSettings& settings, PRNG& generator,
                   std::vector<Obstacle>& Obs_vec);  // adds obstacles to HugeObstacles
void MediumObsSpawn (GenerationSettings& settings, PRNG& generator,
                     std::vector<Obstacle>& Obs_vec);  // adds obstacles to MediumObstacles
void SmallObsSpawn (GenerationSettings& settings, PRNG& generator,
                    std::vector<Obstacle>& Obs_vec);  // adds obstacles to SmallObstacles

bool ObsDistPlayerCheck (GenerationSettings& settings, const Obstacle& Obs,
                         const std::vector<Player>& players_vec);  // obstacle not overlapping player
void ObstaclesRespawn (GenerationSettings& settings, std::vector<Player>& players,
                       std::vector<Obstacle>& Obs_vec);  // regenerates obstacles with existing players

// players generation
bool PlayerDistObsCheck (GenerationSettings& settings, const Graph_lib::Point& Player_pos,
                         const std::vector<Obstacle>& Obs_vec);  // Player not overlapping Obstacles
bool PlayerDistPlayersCheck (GenerationSettings& settings, const Graph_lib::Point& Player_pos,
                             const std::vector<Player>& players_vec);  // Player not overlapping Players

void GenerateObstacles (GenerationSettings& settings,
                        std::vector<Obstacle>& obstacles);  // Generates everything at the start of the game
void GeneratePlayer (GenerationSettings& settings, Player& player_, const std::vector<Obstacle>& obstacles,
                     const std::vector<Player>& players_vec);  // Generates everything at the start of the game

#endif  // TITANFALL_3_LEVEL_GEN_H
