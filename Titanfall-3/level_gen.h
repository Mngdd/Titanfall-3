#ifndef LEVEL_GEN_H
#define LEVEL_GEN_H

#include "environment.h"
#include <Graph_lib/Graph.h>
#include <Graph_lib/Point.h>
#include <random>
#include <vector>

// random tech
struct PRNG {
    std::mt19937 engine;
};

double dist(Graph_lib::Point a, Graph_lib::Point b);// returns dist between two points

void initGenerator(PRNG &generator);                                   // generates seed for random
unsigned random(PRNG &generator, unsigned minValue, unsigned maxValue);// gives random ints

Graph_lib::Point NotsoRandomPoint(double Power);// Point "Power" far away from the center

// obstacles generation
void HugeObsSpawn(std::vector<Obstacle> &Obs_vec);  // adds obstacles to HugeObstacles
void MediumObsSpawn(std::vector<Obstacle> &Obs_vec);// adds obstacles to MediumObstacles
void SmallObsSpawn(std::vector<Obstacle> &Obs_vec); // adds obstacles to SmallObstacles

bool ObsDistPlayerCheck(const Obstacle &Obs, const std::vector<Player> &players_vec);// obstacle not overlapping player
void ObstaclesRespawn(std::vector<Obstacle> &Obs_vec);                               // regenerates obstacles with existing players

// players generation
bool PlayerDistObsCheck(const Graph_lib::Point &Player_pos, const std::vector<Obstacle> &Obs_vec);      // Player not overlapping Obstacles
bool PlayerDistPlayersCheck(const Graph_lib::Point &Player_pos, const std::vector<Player> &players_vec);// Player not overlapping Players

void GenerateObstacles(std::vector<Obstacle> &obstacles);// Generates everything at the start of the game
void GeneratePlayer(Player &player_, const std::vector<Obstacle> &obstacles,
                    const std::vector<Player> &players_vec);// Generates everything at the start of the game

#endif// TITANFALL_3_LEVEL_GEN_H
