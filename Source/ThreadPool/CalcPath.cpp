#include "CalcPath.h"
#include "../Ride/Navigation/PathCalculator.h"
#include "../Ride/Navigation/BFS.h"
#include "../ThreadControl/GlobalInfo.h"

/**
 * Constructor.
 * @param ride ride.
 * @param map city-map.
 * @param lock mutex object.
 * @return CalcPath object.
 */
CalcPath::CalcPath(Ride *ride, CityMap *map, pthread_mutex_t *lock) : ride(
        ride), cityMap(map), locker(lock) {
}

/**
 * Destructor.
 */
CalcPath::~CalcPath() {

}

/**
 * Calculate the path of the ride.
 */
void CalcPath::execute() {
    //Find source point of ride.
    Point srcRidePoint = ride->getSourcePoint();
    Block *src =
            cityMap->getBlock(srcRidePoint.getX(), srcRidePoint.getY());
    // Create navigation.
    PathCalculator *bfs = new BFS(src);
    // Find destination.
    Point destPoint = ride->getDestinationPoint();
    Block *dest = cityMap->getBlock(destPoint.getX(), destPoint.getY());
    bfs->addStoppingPoint(dest);
    pthread_mutex_lock(locker);
    bfs->applyAlgorithm();
    deque<string> *path;
    if (!bfs->isValidRide()) {
        path = NULL;
    } else {
        path = bfs->getPathAsString();
    }
    GlobalInfo *globalInfo = GlobalInfo::getInstance();
    globalInfo->addRideToMap(ride->getId(), path);
    pthread_mutex_unlock(locker);
    delete bfs;
}
