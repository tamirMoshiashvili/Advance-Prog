
#ifndef EX2_GLOBALINFO_H
#define EX2_GLOBALINFO_H

#include <clocale>
#include <map>
using namespace std;

class GlobalInfo {
private:
    int command;
    int driverId;
    map<int, bool> isNewCommandPerDriver;
    map<int, int> descriptorToDriverId;
    // descriptor to bool
    map<int, bool> isDriverFinishedCommand;
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
};

#endif //EX2_GLOBALINFO_H
