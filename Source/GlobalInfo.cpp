
#include "GlobalInfo.h"

GlobalInfo *GlobalInfo::globalInfo = NULL;
bool GlobalInfo::instanceFlag = false;

/**
 * Constructor.
 * @return
 */
GlobalInfo::GlobalInfo() {
    command = 0;
}

/**
 * Create single copy of the object.
 * @return
 */
GlobalInfo *GlobalInfo::getInstance() {
    if (!instanceFlag) {
        globalInfo = new GlobalInfo;
        instanceFlag = true;
        return globalInfo;
    } else {
        // means we already created one before.
        return globalInfo;
    }
}

/**
 * Destructor.
 */
GlobalInfo::~GlobalInfo() {
    instanceFlag = false;
}

/**
 * Add driver to the maps which contain info on this drivers.
 * @param driverId
 * @param socket
 */
void GlobalInfo::addDriverToMap(int driverId, int socket) {
    descriptorToDriverId.insert(pair<int, int>(socket, driverId));
    isDriverFinishedCommand.insert(pair<int, bool>(socket, false));
    isNewCommandPerDriver.insert(pair<int, bool>(socket, false));
}

/**
 * Update the current command given us at the main of server.
 * @param mission
 * @param clientId optional (if the command is 4)
 */
void GlobalInfo::updateCommand(int mission, int clientId) {
    command = mission;
    driverId = clientId;
    map<int, bool>::iterator it;
    for (it = isNewCommandPerDriver.begin();
         it != isNewCommandPerDriver.end(); ++it) {
        it->second = true;
    }
}

/**
 * Get the current command.
 * @return command.
 */
int GlobalInfo::getCurrentCommand() {
    return command;
}

/**
 * Check if after command number 4 was entered the location of the driver with
 * the given socket descriptor was requested.
 * @param driverSocket
 * @return true if it is requested.
 */
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

