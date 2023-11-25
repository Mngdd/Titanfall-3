#ifndef LEVEL_GEN_H
#define LEVEL_GEN_H

// Работает гавкошмыг Юманов Василий Михайлович

// Генерация:
// Сначала генерируются игроки на случайных координатах
// Затем генерируютяс препятствия между игроками
// Затем добавляются случайные препятствия

// Но надо всю генерацию переработать, т.к. без прямых прострелов это почти неиграбельно и скучно и невозможно попасть и вообще ужас

// Вариант генерации:
// Сначала генерируются препятствия, потом игроки
// n - количество игроков
// 1. Сначала появляется n кругов большого радиуса в почти случайном месте таких, что вероятнее будет ближе к краю карты.
// (координата будет браться в квадрате или другой степени больше 1, нужна возможность отрисовки)
// 2. Затем появляется n кругов среднего радиуса так, что что вероятнее всего будут ближе к середине карты
// (кординаты будут браться в степени меньше 1, требуется возможность отрисовки)
// 3. Затем в совершенно случайных местах появляется n маленьких кругов 
// 4. Затем генерируются игроки на малом расстоянии друг от друга

// Карта будет скорее всего 1200:720

// Термины:
// Достаточное расстояние - примерно среднее расстояние как длина большого пальца
// Большое расстояние - по крайней мере четверть карты
// Маленькое расстояние
//
// При смерти игрок появляется на достаточном расстоянии от противников

#include <cmath>
#include "std_lib_facilities.h"

void HugeObsSpawn()     // adds obstacles to HugeObstacles
void MediumObsSpawn()   // adds obstacles to MediumObstacles
void SmallObsSpawn()    // adds obstacles to SmallObstacles
void PlayersSpawn()     // adds players to Players

extern vector<Circle> HugeObstacles;
extern vector<Circle> MediumObstacles;
extern vector<Circle> SmallObstacles;

extern vector<Circle> Players;

#endif //TITANFALL_3_LEVEL_GEN_H