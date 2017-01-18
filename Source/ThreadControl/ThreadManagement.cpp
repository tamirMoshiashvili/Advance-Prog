#include <boost/log/trivial.hpp>
#include "ThreadManagement.h"

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
    GlobalInfo *globalInfo = clientThreadInfo->globalInfo;
//    pthread_mutex_t *map_insertion_locker = clientThreadInfo->map_insertion_locker;
    pthread_mutex_t *map_insertion_locker =globalInfo->getLocker();
    pthread_mutex_init(map_insertion_locker, 0);
    // Get driver-id and its cab id.
    center->identifyDriver(driverSocket, globalInfo);
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
                BOOST_LOG_TRIVIAL(debug) << "driver " << driverSocket
                                         << " enter 9";
                // Operate the driver.
                center->makeDriverWork(driverSocket);
                break;
            default:
                break;
        }
        if (globalInfo->getIsNewCommand(driverSocket)) {
            // Get ready for the next mission.
//            pthread_mutex_lock(map_insertion_locker);
            BOOST_LOG_TRIVIAL(debug) << "driver with the num of socket: "
                                     << driverSocket
                                     << " finished";
            globalInfo->setNotNewCommand(driverSocket);
//            pthread_mutex_unlock(map_insertion_locker);
            // Wait for new mission.
            while (!globalInfo->getIsNewCommand(driverSocket)) {
            }
        }
        // Get new command.
        command = globalInfo->getCurrentCommand(driverSocket);
    }
    delete clientThreadInfo;
}


