//
// Created by khris on 22.11.2023.
//
#include <bits/stdc++.h>
#include "enviroment.h"

using namespace std;
// что я должен делать когда хп меньше 0
class Player
{private:
    string name;
    double health;
    double damage;
    int x, y;
    
public:

    Player(string namePlayer, double health_point, int xcord, int ycord){
        name = namePlayer;
        health = health_point;
        x = xcord;
        y = ycord;}

    double GetHealth(){
        return health;
    }

    int GetCords(){
        return x, y;
    }

    void SetDamage(double damagevalue){
        damage = damagevalue;
        health -= damage;
        if (health <= 0){GetHealth();}}
};

class Func_trace
{

};



