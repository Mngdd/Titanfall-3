#ifndef LEVEL_GEN_H
#define LEVEL_GEN_H

#include <random>
#include <Graph_lib/Point.h>
#include "environment.h"

// random tech
struct PRNG
{
    std::mt19937 engine;
};
void initGenerator(PRNG& generator);    // generates seed for random
unsigned random(PRNG& generator, unsigned minValue, unsigned maxValue); // gives random ints

Point NotsoRandomPoint(double Power);   // Point "Power" far away from the center

// obstacles generation
void HugeObsSpawn ();    // adds obstacles to HugeObstacles
void MediumObsSpawn ();  // adds obstacles to MediumObstacles
void SmallObsSpawn ();   // adds obstacles to SmallObstacles

bool ObsDistPlayerCheck(Obstacle const *Obstacle&);  // obstacle not overlapping player
void ObstaclesRespawn(); // regenerates obstacles with existing players

// players generation
bool PlayerDistObsCheck(Point const Player&);      // Player not overlapping Obstacles
bool PlayerDistPlayersCheck(Point const Player&);  // Player not overlapping Players
void PlayerSpawn();      // adds player to Players
void PlayersSpawn ();    // adds players to Players

void Generate();        // Generates everything at the start of the game

extern std::vector<Obstacle*> Obstacles;    // vector of all Obstacles
extern std::vector<Point*> Players;         // vector of all players

#endif  // TITANFALL_3_LEVEL_GEN_H