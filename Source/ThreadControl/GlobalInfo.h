#ifndef EX2_GLOBALINFO_H
#define EX2_GLOBALINFO_H

#include <clocale>
#include <map>
#include <mutex>
using namespace std;

/**
 * Singleton class which contain all global data that every thread needs.
 */
class GlobalInfo {
private:
    int command;
    int driverId;
    map<int, bool> isNewCommandPerDriver;
    map<int, int> descriptorToDriverId;
    // descriptor to bool
    map<int, bool> isDriverFinishedCommand;
    pthread_mutex_t lock;
    static bool instanceFlag;
    static GlobalInfo *globalInfo;

    GlobalInfo();
public:
    static GlobalInfo *getInstance();

    ~GlobalInfo();

    void addDriverToMap(int driverId, int socket);

    void updateCommand(int mission, int clientId = -1);

    int getCurrentCommand();

    bool isDriverLocationRequested(int driverSocket);

    bool areAllDriversFinishedCommand();

    void setAllDriversToNotFinish();

    void setDriverFinishCommand(int driverSocket);

    void setNotNewCommand(int driverSocket);

    bool getIsNewCommand(int driverSocket);

    bool isDriverFinishCommand(int driverSocket);

    unsigned long getNumClients();
};

#endif //EX2_GLOBALINFO_H
