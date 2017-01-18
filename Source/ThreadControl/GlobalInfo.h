#ifndef EX2_GLOBALINFO_H
#define EX2_GLOBALINFO_H

#include <clocale>
#include <map>
#include <deque>
#include <mutex>

using namespace std;

/**
 * Singleton class which contain all global data that every thread needs.
 */
class GlobalInfo {
private:
    int driverId;
    map<int, int> commandPerDriver;
    map<int, int> descriptorToDriverId;
    map<int, bool> driverToFlag;
    map<int, deque<string> *> rideIdToPath;
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

    void turnOffFlag(int driverSocket);

    bool isFlagTurnOn();

    void addRideToMap(int rideId, deque<string> *string_path);

    bool doesRideExist(int rideId);

    deque<string> *popPathOf(int rideId);
};

#endif //EX2_GLOBALINFO_H

