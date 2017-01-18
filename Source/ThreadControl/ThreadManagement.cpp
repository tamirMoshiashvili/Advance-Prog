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
    BOOST_LOG_TRIVIAL(debug) << "Start THREAD FUNC with driver: " << driverSocket << endl;
    TaxiCenter *center = clientThreadInfo->taxiCenter;
    GlobalInfo *globalInfo = clientThreadInfo->globalInfo;
    pthread_mutex_t *map_insertion_locker =
            clientThreadInfo->map_insertion_locker;
    pthread_mutex_init(map_insertion_locker, 0);
    // Get driver-id and its cab id.
    center->identifyDriver(driverSocket, globalInfo);
    int command = globalInfo->getCurrentCommand();
    while (command != 7) {
        switch (command) {
            case 4:
                BOOST_LOG_TRIVIAL(debug) << "Start THREAD FUNC-case 4 with driver: " << driverSocket;
                // Check if client's location is needed.
                if (globalInfo->isDriverLocationRequested(driverSocket)) {
                    Point point = center->askDriverLocation(driverSocket);
                    BOOST_LOG_TRIVIAL(debug) << point;
                }
                BOOST_LOG_TRIVIAL(debug) << "End THREAD FUNC-case 4 with driver: " << driverSocket << endl;
                break;
            case 9:
                BOOST_LOG_TRIVIAL(debug) << "Start THREAD FUNC-case 9 with driver: " << driverSocket;
                // Operate the driver.
                center->makeDriverWork(driverSocket);
                BOOST_LOG_TRIVIAL(debug) << "End THREAD FUNC-case 9 with driver: " << driverSocket << endl;
                break;
            default:
                break;
        }
        // Get ready for the next mission.
        pthread_mutex_lock(map_insertion_locker);
        globalInfo->setDriverFinishCommand(driverSocket);
        globalInfo->setNotNewCommand(driverSocket);
        BOOST_LOG_TRIVIAL(debug) << "driver with the num of socket: " << driverSocket << " finished";
        pthread_mutex_unlock(map_insertion_locker);
        // Wait for new mission.
        BOOST_LOG_TRIVIAL(debug) << "Wait for new command with driver: " << driverSocket;
        while (!globalInfo->getIsNewCommand(driverSocket)) {
        }
        // Get new command.
        command = globalInfo->getCurrentCommand();
        BOOST_LOG_TRIVIAL(debug) << "GOT new command with driver: " << driverSocket << endl;
    }
    delete clientThreadInfo;
}


