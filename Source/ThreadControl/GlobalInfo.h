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
    int driverId;
    bool flag;
    map<int, int> commandPerDriver;
    map<int, int> descriptorToDriverId;
    map<int, bool> driverToFlag;
    pthread_mutex_t lock;
    static bool instanceFlag;
    static GlobalInfo *globalInfo;

    GlobalInfo();
public:
    static GlobalInfo *getInstance();

    ~GlobalInfo();

    void addDriverToMap(int socket);

    void addToSocketToIdMap(int driverId, int socket);

    void updateCommand(int mission, int clientId = -1);

    int getCurrentCommand(int driverSocket);

    bool isDriverLocationRequested(int driverSocket);

    bool areAllDriversFinishedCommand();

    void setNotNewCommand(int driverSocket);

    bool getIsNewCommand(int driverSocket);

    pthread_mutex_t* getLocker();

    void turnOnFlag();

    void turnOffFlag(int driverSocket);

    bool isFlagTurnOn();

    unsigned long getNumClients();
};

#endif //EX2_GLOBALINFO_H

