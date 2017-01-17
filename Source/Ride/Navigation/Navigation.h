#ifndef EX2_NAVIGATION_H
#define EX2_NAVIGATION_H


#include <deque>
#include "../../Basic/Point.h"
#include "../../Basic/Block/Block.h"

using namespace std;

class Navigation {
private:
    deque<Point> *path;

public:
    Navigation(deque<Block *> *blockPath);

    ~Navigation();

    Point getNextLocation();

    bool hasNextLocation();

};


#endif //EX2_NAVIGATION_H
