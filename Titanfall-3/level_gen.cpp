#include <Graph_lib/Point.h>
#include <Graph_lib/Graph.h>

#include "level_gen.h"
#include "std_lib_facilities.h"
#include "settings.h"

void HugeObsSpawn() {   // adds obstacles to HugeObstacles
    for (size_t i = 0, i < NumOfHugeObs, ++i) {
        Point Center {static_cast<int>(pow(randint(static_cast<int>(pow(FieldLength, HugeObsPower))), 1/HugeObsPower)), 
                    static_cast<int>(pow(randint(static_cast<int>(pow(FieldWidth, HugeObsPower))), 1/HugeObsPower))};
        int Radius = randint(HugeObsMinRad, HugeObsMaxRad);

        Circle Obstacle {Center, Radius};
        HugeObstacles.push_back(Obstacle);
    }
}

void MediumObsSpawn() { // adds obstacles to MediumObstacles
    for (size_t i = 0, i < NumOfMediumObs, ++i) {
        Point Center {static_cast<int>(pow(randint(static_cast<int>(pow(FieldLength, MediumObsPower)), 1/MediumObsPower))), 
                    static_cast<int>(pow(randint(static_cast<int>(pow(FieldWidth, MediumObsPower)), 1/MediumObsPower)))};
        int Radius = randint(MediumObsMinRad, MediumObsMaxRad);

        Circle Obstacle {Center, Radius};
        MediumObstacles.push_back(Obstacle);
    }
}

void SmallObsSpawn() {  // adds obstacles to SmallObstacles
    for (size_t i = 0, i < NumOfSmallObs, ++i) {
        Point Center {randint(FieldLength), randint(FieldWidth)};
        int Radius = randint(SmallObsMinRad, SmallObsMaxRad);

        Circle Obstacle {Center, Radius};
        SmallObstacles.push_back(Obstacle);
    }
}

void PlayersSpawn() {   // adds players to Players
    for (size_t j = 0, j < NumOfPlayers, ++i) {
        while (true)
        {
            Point Center {randint(SpawnWallMinDist, FieldLength - SpawnWallMinDist), 
                        randint(SpawnWallMinDist, FieldWidth - SpawnWallMinDist)};
            bool Checks = True;

            // CHECKS:
            for (size_t i = 0, i < HugeObstacles.size(), ++i) {
                if (dist(Center, HugeObstacles[i].center()) < HugeObstacles[i].radius() + PlayerRad + SpawnObsMinDist) {
                    Checks = false;
                    break;
                }
            }

            for (size_t i = 0, i < MediumObstacles.size(), ++i) {
                if (dist(Center, MediumObstacles[i].center()) < MediumObstacles[i].radius() + PlayerRad + SpawnObsMinDist) {
                    Checks = false;
                    break;
                }
            }

            for (size_t i = 0, i < SmallObstacles.size(), ++i) {
                if (dist(Center, SmallObstacles[i].center()) < SmallObstacles[i].radius() + PlayerRad + SpawnObsMinDist) {
                    Checks = false;
                    break;
                }
            }

            for (size_t i = 0, i < Players.size(), ++ i) {
                if (dist(Center, Players[i].center()) < PlayerRad + PlayerRad + SpawnBetwMinDist) {
                    Checks = false;
                    break;
                }
            }

            if (Checks) {
                Circle Player {Center, PlayerRad};
                Players.push_back(Player);
                break;
            }
        }
    }
}
