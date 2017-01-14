
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
    cout << "enter thread with socket descriptor: " << driverSocket << "\n";
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
                cout << "make driver with socket: " << driverSocket
                     << " work\n";
                center->makeDriverWork(driverSocket,
                                       clientThreadInfo->map_itearation_locker);
                break;
            default:
                break;
        }
        pthread_mutex_lock(map_insertion_locker);
        globalInfo->setDriverFinishCommand(driverSocket);
        globalInfo->setNotNewCommand();
        cout << "driver with the num of socket: " << driverSocket
             << " finished\n";
        while (!globalInfo->getIsNewCommand()) {
        }
        pthread_mutex_unlock(map_insertion_locker);
        command = globalInfo->getCurrentCommand();
    }
}

