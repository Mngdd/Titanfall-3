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
void HugeObsSpawn();  // adds obstacles to HugeObstacles
void MediumObsSpawn();// adds obstacles to MediumObstacles
void SmallObsSpawn(); // adds obstacles to SmallObstacles

bool ObsDistPlayerCheck(const Obstacle &Obs);// obstacle not overlapping player
void ObstaclesRespawn();                     // regenerates obstacles with existing players

// players generation
bool PlayerDistObsCheck(const Graph_lib::Point &Player);    // Player not overlapping Obstacles
bool PlayerDistPlayersCheck(const Graph_lib::Point &Player);// Player not overlapping Players
void PlayerSpawn();                              // adds player to Players
void PlayersSpawn(int NumOfPlayersAdded);        // adds players to Players

void Generate();// Generates everything at the start of the game

extern std::vector<Obstacle> Obstacles;// vector of all Obstacles
extern std::vector<Graph_lib::Point> Players;      // vector of all players

#endif// TITANFALL_3_LEVEL_GEN_H
