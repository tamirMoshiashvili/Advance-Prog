#include "Navigation.h"

/**
 * Constructor.
 * @param blockPath deque of strings, which represents the path as strings.
 * @return Navigation object.
 */
Navigation::Navigation(deque<string> *blockPath) {
    path = new deque<Point>();
    while (!blockPath->empty()) {
        string point_str = blockPath->front();
        blockPath->pop_front();
        path->push_back(Point::strToPoint(point_str));
    }
}

/**
 * Default constructor.
 * @return Navigation object.
 */
Navigation::Navigation() : path(NULL) {}

/**
 * Destructor.
 */
Navigation::~Navigation() {
    delete path;
}

/**
 * Get the next location in the way.
 * @return point.
 */
Point Navigation::getNextLocation() {
    Point next = path->back();
    path->pop_back();
    return next;
}

/**
 * Check if need to continue in the path (path is not over).
 * @return true if path is not empty, true otherwise.
 */
bool Navigation::hasNextLocation() {
    if (!path->empty()) {
        Point block = path->back();
        if (block == Point(-1, -1)) {
            path->pop_back();
            return false;
        }
        return true;
    }
    return false;
}
