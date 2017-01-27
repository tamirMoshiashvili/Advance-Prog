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
    GlobalInfo *globalInfo = GlobalInfo::getInstance();
    // Get driver-id and its cab id.
    center->identifyDriver(driverSocket);
    globalInfo->turnOffFlag(driverSocket);
    int command = globalInfo->getCurrentCommand(driverSocket);
    while (command != 7) {
        switch (command) {
            case 4:
                cout << "Command 4 with des: " << driverSocket << "\n";
                // Check if client's location is needed.
                if (globalInfo->isDriverLocationRequested(driverSocket)) {
                    Point point = center->askDriverLocation(driverSocket);
                    cout << point << endl;
                }
                break;
            case 9:
                cout << "Command 9 with des: " << driverSocket << "\n";
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
            cout << "Wait for new command...\n";
            while (!globalInfo->getIsNewCommand(driverSocket)) {
            }
        }
        // Get new command.
        command = globalInfo->getCurrentCommand(driverSocket);
    }
    delete clientThreadInfo;
    pthread_exit(NULL);
}
