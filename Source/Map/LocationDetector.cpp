#include "LocationDetector.h"

/**
 * Constructor.
 * @param cityMap map, represents the space.
 * @return LocationDetector object.
 */
LocationDetector::LocationDetector(CityMap *cityMap) : map(cityMap) {

}

/**
 * Default constructor.
 * @return LocationDetector object.
 */
LocationDetector::LocationDetector() : map(NULL) {

}

/**
 * Destructor.
 */
LocationDetector::~LocationDetector() {
    if (map != NULL) {
        delete map;
        map = NULL;
    }
}

/**
 * Get the locatoin of the given block in the map.
 * @param block pointer to block.
 * @return point location of the block.
 */
Point LocationDetector::getLocation(Block *block) {
    // Run through the map and find the block.
    int width = map->getWidth();
    int height = map->getHeight();
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (*map->getBlock(i, j) == *block) {
                // Block has been found, send the point.
                return Point(i, j);
            }
        }
    }
    // Block has not found.
    return Point(-1, -1);
}

/**
 * Get the pointer of the block with the current id.
 * @param id id number of a block.
 * @return pointer to block.
 */
Block *LocationDetector::getBlock(int id) {
    // Run through the map and find the block.
    int width = map->getWidth();
    int height = map->getHeight();
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (map->getBlock(i, j)->getId() == id) {
                // Block has been found, send it.
                return map->getBlock(i, j);
            }
        }
    }
    // Block has not found.
    return NULL;
}
