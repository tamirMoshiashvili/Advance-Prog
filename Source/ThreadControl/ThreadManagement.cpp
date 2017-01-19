#include "ThreadManagement.h"
#include "../Ride/Navigation/BFS.h"

/**
 * Check for a mission to execute and contact with the needed client,
 * and operate him according to the mission.
 * @param param struct of ClientThreadInfo.
 */
void *ThreadManagement::threadFunction(void *param) {
    // Initialization.
    ClientThreadInfo *clientThreadInfo = (ClientThreadInfo *) param;
    int driverSocket = clientThreadInfo->socket;
    TaxiCenter *center = clientThreadInfo->taxiCenter;
    GlobalInfo *globalInfo = GlobalInfo::getInstance();
    // Get driver-id and its cab id.
    center->identifyDriver(driverSocket);
    globalInfo->turnOffFlag(driverSocket);
    int command = globalInfo->getCurrentCommand(driverSocket);
    while (command != 7) {
        switch (command) {
            case 4:
                // Check if client's location is needed.
                if (globalInfo->isDriverLocationRequested(driverSocket)) {
                    Point point = center->askDriverLocation(driverSocket);
                    cout << point << endl;
                }
                break;
            case 9:
                // Operate the driver.
                center->makeDriverWork(driverSocket);
                break;
            default:
                break;
        }
        if (globalInfo->getIsNewCommand(driverSocket)) {
            // Get ready for the next mission.
            globalInfo->setNotNewCommand(driverSocket);
            // Wait for new mission.
            while (!globalInfo->getIsNewCommand(driverSocket)) {
            }
        }
        // Get new command.
        command = globalInfo->getCurrentCommand(driverSocket);
    }
    delete clientThreadInfo;
    pthread_exit(NULL);
}

/**
 * Create navigation.
 * @param param path calc info.
 */
void *ThreadManagement::produceNavigation(void *param) {
    PathCalcInfo *info = (PathCalcInfo *) param;
    Ride *ride = info->ride;
    CityMap *cityMap = info->cityMap;
    // Find source of the ride.
    pthread_mutex_t locker;
    pthread_mutex_init(&locker, 0);
    pthread_mutex_lock(&locker);
    Point srcRidePoint = ride->getSourcePoint();
    Block *src =
            cityMap->getBlock(srcRidePoint.getX(), srcRidePoint.getY());
    // Create navigation.
    PathCalculator *bfs = new BFS(src);
    // Find destination.
    Point destPoint = ride->getDestinationPoint();
    Block *dest = cityMap->getBlock(destPoint.getX(), destPoint.getY());
    bfs->addStoppingPoint(dest);
    bfs->applyAlgorithm();
    GlobalInfo *globalInfo = GlobalInfo::getInstance();
    globalInfo->addRideToMap(ride->getId(), bfs->getPathAsString());
    pthread_mutex_destroy(&locker);
    delete bfs;
    delete info;
    pthread_exit(NULL);
}
