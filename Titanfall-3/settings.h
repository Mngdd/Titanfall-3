#ifndef SETTINGS_H
#define SETTINGS_H

const int FieldLength = 1200;       // Длина поля
const int FieldWidth = 720;         // Ширина поля


// ГЕНЕРАЦИЯ

// УБРАТЬ const ПЕРЕД ТЕМИ НАСТРОЙКАМИ, ЧТО МОЖНО БУДЕТ МЕНЯТЬ В МЕНЮШКЕ!!

int NumOfPlayers;                   // ТРЕБУЕТСЯ ЗАДАВАТЬ

const int HugeObsPower = 2;         // Степень ухода больших препятствий от центра
const int MediumObsPower = 0.5;     // Степень близости средних препятствий к центру

const int SpawnBetwMinDist = 70;   // Минимальное расстояние между игроками (соло режим)
const int SpawnWallMinDist = 30;    // Минимальное расстояние от игрока до стены (больше радиуса игрока)
const int SpawnObsMinDist = 5;      // Минимальное расстояние от игрока до препятствия

const int PlayerRad = 15;           // Радиус игрока

const int NumOfHugeObs = 5;         // Количество препятствий каждого вида
const int NumOfMediumObs = 5;
const int NumOfSmallObs = 5;

const int WhiteObsRad = 5;          // радиус вырезов препятсвий
const int HugeObsMaxRad = 100;      // Радиусы препятствий разных размеров
const int HugeObsMinRad = 65;
const int MediumObsMaxRad = 65;
const int MediumObsMinRad = 30;
const int SmallObsMaxRad = 30;
const int SmallObsMinRad = 15;

#endif