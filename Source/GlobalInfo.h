
#ifndef EX2_GLOBALINFO_H
#define EX2_GLOBALINFO_H

#include <clocale>
#include <map>
using namespace std;

class GlobalInfo {
private:
    int command;
    bool isNewCommand;
    int driverId;
    map<int, int> descriptorToDriverId;
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

    bool isDriverFinished(int driverSocket);

    void setNotNewCommand();

    bool getIsNewCommand();
};

#endif //EX2_GLOBALINFO_H
