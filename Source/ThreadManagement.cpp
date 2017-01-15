
#include "ThreadManagement.h"


ThreadManagement::ThreadManagement() {
}

ThreadManagement::~ThreadManagement() {

}

void *ThreadManagement::threadFunction(void *param) {
    ClientThreadInfo *clientThreadInfo = (ClientThreadInfo *) param;
    int driverSocket = clientThreadInfo->socket;
    TaxiCenter *center = clientThreadInfo->taxiCenter;
    GlobalInfo *globalInfo = clientThreadInfo->globalInfo;
    pthread_mutex_t *map_insertion_locker = clientThreadInfo->map_insertion_locker;
    pthread_mutex_init(map_insertion_locker, 0);
    // Get driver id and its cab id.
    center->identifyDriver(driverSocket, globalInfo);
    int command = globalInfo->getCurrentCommand();
    while (command != 7) {
        switch (command) {
            case 4:
                if (globalInfo->isDriverLocationRequested(driverSocket)) {
                    Point point = center->askDriverLocation(driverSocket);
                    cout << point << "\n";
                }
                break;
            case 9:
                center->makeDriverWork(driverSocket);
                break;
            default:
                break;
        }
        pthread_mutex_lock(map_insertion_locker);
        globalInfo->setDriverFinishCommand(driverSocket);
        globalInfo->setNotNewCommand(driverSocket);
        cout << "driver with the num of socket: " << driverSocket
             << " finished\n";
        pthread_mutex_unlock(map_insertion_locker);
        while (!globalInfo->getIsNewCommand(driverSocket)) {
        }
        // Get new command.
        command = globalInfo->getCurrentCommand();
    }
//    delete map_insertion_locker;
//    delete map_iteration_lock;
    delete clientThreadInfo;
}


