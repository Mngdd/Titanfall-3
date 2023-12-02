#include <Graph_lib/Point.h>

using namespace Graph_lib;

struct Obstacle {
    Point center;
    int radius;
    bool hole;
}

int GetCords();

double GetHealth();

void SetDamage(double damagevalue);

#endif //TITANFALL_3_ENVIRONMENT_H
