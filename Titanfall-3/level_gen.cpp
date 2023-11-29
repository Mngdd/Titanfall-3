<<<<<<< Updated upstream

#include <Graph_lib/Point.h>

=======
>>>>>>> Stashed changes
#include "level_gen.h"
#include "settings.h"

<<<<<<< Updated upstream
void HugeObsSpawn() {   // adds obs tacles to HugeObstacles
    for (size_t i = 0; i < NumOfHugeObs; ++i) {
        Point Center {static_cast<int>(pow(Graph_lib::randint(static_cast<int>(pow(FieldLength, HugeObsPower))), 1/HugeObsPower)), 
                    static_cast<int>(pow(Graph_lib::randint(static_cast<int>(pow(FieldWidth, HugeObsPower))), 1/HugeObsPower))};
        int Radius = Graph_lib::randint(HugeObsMinRad, HugeObsMaxRad);
=======
vector<Circle*> HugeObstacles;
vector<Circle*> MediumObstacles;
vector<Circle*> SmallObstacles;
>>>>>>> Stashed changes

vector<Circle*> Players;


void HugeObsSpawn() {   // adds obstacles to HugeObstacles
    for (size_t i = 0; i < NumOfHugeObs; ++i) {
        Point Center {static_cast<int>(pow(Graph_lib::randint(static_cast<int>(pow(FieldLength, HugeObsPower))), 1/HugeObsPower)), 
                    static_cast<int>(pow(Graph_lib::randint(static_cast<int>(pow(FieldWidth, HugeObsPower))), 1/HugeObsPower))};
        int Radius = Graph_lib::randint(HugeObsMinRad, HugeObsMaxRad);
        
        Circle* Obstacle = new Circle {Center, Radius};
        (*Obstacle).set_fill_color(Color::black);
        HugeObstacles.push_back(Obstacle);
    }
}

void MediumObsSpawn() { // adds obstacles to MediumObstacles
    for (size_t i = 0; i < NumOfMediumObs; ++i) {
<<<<<<< Updated upstream
        Point Center {static_cast<int>(pow(Graph_lib::randint(static_cast<int>(pow(FieldLength, MediumObsPower)), 1/MediumObsPower))), 
                    static_cast<int>(pow(Graph_lib::randint(static_cast<int>(pow(FieldWidth, MediumObsPower)), 1/MediumObsPower)))};
        int Radius = Graph_lib::randint(MediumObsMinRad, MediumObsMaxRad);
=======
        Point Center {static_cast<int>(pow(Graph_lib::randint(static_cast<int>(pow(FieldLength, MediumObsPower))), 1/MediumObsPower)), 
                    static_cast<int>(pow(Graph_lib::randint(static_cast<int>(pow(FieldWidth, MediumObsPower))), 1/MediumObsPower))};
        int Radius = Graph_lib::randint(MediumObsMinRad, MediumObsMaxRad);
        
        Circle* Obstacle = new Circle {Center, Radius};
        (*Obstacle).set_fill_color(Color::black);
>>>>>>> Stashed changes

        MediumObstacles.push_back(Obstacle);
    }
}

void SmallObsSpawn() {  // adds obstacles to SmallObstacles
    for (size_t i = 0; i < NumOfSmallObs; ++i) {
        Point Center {Graph_lib::randint(FieldLength), Graph_lib::randint(FieldWidth)};
        int Radius = Graph_lib::randint(SmallObsMinRad, SmallObsMaxRad);
<<<<<<< Updated upstream
=======

        Circle* Obstacle = new Circle {Center, Radius};
        (*Obstacle).set_fill_color(Color::black);
>>>>>>> Stashed changes

        SmallObstacles.push_back(Obstacle);
    }
}

void PlayersSpawn() {   // adds players to Players
<<<<<<< Updated upstream
    for (size_t j = 0; j < NumOfPlayers; ++i) {
=======
    for (size_t j = 0; j < NumOfPlayers; ++j) {
>>>>>>> Stashed changes
        while (true)
        {
            Point Center {Graph_lib::randint(SpawnWallMinDist, FieldLength - SpawnWallMinDist), 
                        Graph_lib::randint(SpawnWallMinDist, FieldWidth - SpawnWallMinDist)};
            bool Checks = true;

            // CHECKS:
            for (size_t i = 0; i < HugeObstacles.size(); ++i) {
<<<<<<< Updated upstream
                if (dist(Center, HugeObstacles[i].center()) < HugeObstacles[i].radius() + PlayerRad + SpawnObsMinDist) {
=======
                if (dist(Center, (*HugeObstacles[i]).center()) < (*HugeObstacles[i]).radius() + PlayerRad + SpawnObsMinDist) {
>>>>>>> Stashed changes
                    Checks = false;
                    break;
                }
            }

            for (size_t i = 0; i < MediumObstacles.size(); ++i) {
<<<<<<< Updated upstream
                if (dist(Center, MediumObstacles[i].center()) < MediumObstacles[i].radius() + PlayerRad + SpawnObsMinDist) {
=======
                if (dist(Center, (*MediumObstacles[i]).center()) < (*MediumObstacles[i]).radius() + PlayerRad + SpawnObsMinDist) {
>>>>>>> Stashed changes
                    Checks = false;
                    break;
                }
            }

            for (size_t i = 0; i < SmallObstacles.size(); ++i) {
<<<<<<< Updated upstream
                if (dist(Center, SmallObstacles[i].center()) < SmallObstacles[i].radius() + PlayerRad + SpawnObsMinDist) {
=======
                if (dist(Center, (*SmallObstacles[i]).center()) < (*SmallObstacles[i]).radius() + PlayerRad + SpawnObsMinDist) {
>>>>>>> Stashed changes
                    Checks = false;
                    break;
                }
            }

            for (size_t i = 0; i < Players.size(); ++ i) {
<<<<<<< Updated upstream
                if (dist(Center, Players[i].center()) < PlayerRad + PlayerRad + SpawnBetwMinDist) {
=======
                if (dist(Center, (*Players[i]).center()) < PlayerRad + PlayerRad + SpawnBetwMinDist) {
>>>>>>> Stashed changes
                    Checks = false;
                    break;
                }
            }

            if (Checks) {
                Circle* Player = new Circle {Center, PlayerRad};
                (*Player).set_fill_color(Color::dark_green);
                Players.push_back(Player);
                break;
            }
        }
    }
}
