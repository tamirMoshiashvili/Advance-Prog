#include "Navigation.h"

Navigation::Navigation(deque<Block *> *blockPath) {
    path = new deque<Point>();
}

Navigation::~Navigation() {
    delete path;
}

Point Navigation::getNextLocation() {
    Point next = path->back();
    path->pop_back();
    return next;
}

bool Navigation::hasNextLocation() {

}
