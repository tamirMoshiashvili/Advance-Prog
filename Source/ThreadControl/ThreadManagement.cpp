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
                BOOST_LOG_TRIVIAL(debug) << "driver " << driverSocket << " enter 9";
                // Operate the driver.
                center->makeDriverWork(driverSocket);
                break;
            default:
                break;
        }
        if (globalInfo->getIsNewCommand(driverSocket)) {
            // Get ready for the next mission.
            BOOST_LOG_TRIVIAL(debug) << "driver with the num of socket: "
                                     << driverSocket << " finished";
            globalInfo->setNotNewCommand(driverSocket);
            // Wait for new mission.
            while (!globalInfo->getIsNewCommand(driverSocket)) {
            }
        }
        // Get new command.
        command = globalInfo->getCurrentCommand(driverSocket);
    }
    delete clientThreadInfo;
}
