#ifndef SETTINGS_H
#define SETTINGS_H

#include <Graph_lib/Graph.h>

const int FieldLength = 1200; // Длина поля
const int FieldWidth = 720;   // Ширина поля

// РАСКРАСКА

Color HugeObsColor = Color::black;
Color MediumObsColor = Color::black;
Color SmallObsColor = Color::black;

bool RainbowPlayers = true; // Случайный цвет игроков
Color PlayerColor = Color::green;
// Color Team1PlayerColor = Color::blue;
// Color Team2PlayerColor = Color::red;

// ГЕНЕРАЦИЯ

// УБРАТЬ const ПЕРЕД ТЕМИ НАСТРОЙКАМИ, ЧТО МОЖНО БУДЕТ МЕНЯТЬ В МЕНЮШКЕ!!

int NumOfPlayers = 2; // ТРЕБУЕТСЯ ЗАДАВАТЬ

const int PlayerRad = 10; // Радиус игрока

const double HugeObsPower = 2;     // Степень ухода больших препятствий от центра
const double MediumObsPower = 0.5; // Степень ухода средних препятствий от центра
const double SmallObsPower = 1;    // Степень ухода маленьких препятствий от центра

const int SpawnBetwMinDist = 70; // Минимальное расстояние между игроками (соло режим)
const int SpawnWallMinDist = 30; // Минимальное расстояние от игрока до стены (больше радиуса игрока)
const int SpawnObsMinDist = 5;   // Минимальное расстояние от игрока до препятствия

const int SpawnObsWallMinDist = 3; // Минимальное расстояние от препятствия до стены

const int NumOfHugeObs = 5; // Количество препятствий каждого вида
const int NumOfMediumObs = 5;
const int NumOfSmallObs = 5;

const int HugeObsMaxRad = 100; // Радиусы препятствий разных размеров
const int HugeObsMinRad = 65;
const int MediumObsMaxRad = 65;
const int MediumObsMinRad = 30;
const int SmallObsMaxRad = 30;
const int SmallObsMinRad = 15;

const int PlayerSpawnTries = 100;     // Количество попыток заспавнить игрока в норм месте
const int ObstacleReSpawnTries = 100; // Количество попыток зареспавнить препятствие в норм месте

#endif
