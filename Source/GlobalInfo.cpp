
#include "GlobalInfo.h"

GlobalInfo *GlobalInfo::globalInfo = NULL;
bool GlobalInfo::instanceFlag = false;


GlobalInfo::GlobalInfo() {

}

GlobalInfo *GlobalInfo::getInstance() {
    if (!instanceFlag) {
        globalInfo = new GlobalInfo;
        instanceFlag = true;
        return globalInfo;
    } else {
        return globalInfo;
    }
}

GlobalInfo::~GlobalInfo() {
    instanceFlag = false;
}

void GlobalInfo::addDriverToMap(int driverId, int socket) {
    descriptorToDriverId.insert(pair<int, int>(socket, driverId));
    isDriverFinishedCommand.insert(pair<int, bool>(socket, false));
    isNewCommandPerDriver.insert(pair<int, bool>(socket, false));
}

void GlobalInfo::updateCommand(int mission, int clientId) {
    command = mission;
    driverId = clientId;
    map<int, bool>::iterator it;
    for (it = isNewCommandPerDriver.begin();
         it != isNewCommandPerDriver.end(); ++it) {
        it->second = true;
    }
}

int GlobalInfo::getCurrentCommand() {
    return command;
}

bool GlobalInfo::isDriverLocationRequested(int driverSocket) {
    return descriptorToDriverId.at(driverSocket) == driverId;
}

bool GlobalInfo::areAllDriversFinishedCommand() {
    map<int, bool>::iterator it;
    for (it = isDriverFinishedCommand.begin();
         it != isDriverFinishedCommand.end(); ++it) {
        if (!it->second) {
            return false;
        }
    }
    return true;
}

void GlobalInfo::setAllDriversToNotFinish() {
    map<int, bool>::iterator it;
    for (it = isDriverFinishedCommand.begin();
         it != isDriverFinishedCommand.end(); ++it) {
        it->second = false;
    }
}

void GlobalInfo::setDriverFinishCommand(int driverSocket) {
    isDriverFinishedCommand.at(driverSocket) = true;
}

bool GlobalInfo::getIsNewCommand(int driverSocket) {
    return isNewCommandPerDriver.at(driverSocket);
}

void GlobalInfo::setNotNewCommand(int driverSocket) {
    isNewCommandPerDriver.at(driverSocket) = false;
}

bool GlobalInfo::isDriverFinishCommand(int driverSocket) {
    return isDriverFinishedCommand.at(driverSocket);
}

