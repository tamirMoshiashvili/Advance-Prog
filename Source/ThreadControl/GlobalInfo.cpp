#include "GlobalInfo.h"

GlobalInfo *GlobalInfo::globalInfo = NULL;
bool GlobalInfo::instanceFlag = false;

/**
 * Constructor.
 * @return GlobalInfo object.
 */
GlobalInfo::GlobalInfo() {
    command = 0;
    pthread_mutex_init(&lock, 0);
}

/**
 * Create single copy of the object.
 * @return GlobalInfo object.
 */
GlobalInfo *GlobalInfo::getInstance() {
    if (!instanceFlag) {
        globalInfo = new GlobalInfo;
        instanceFlag = true;
        return globalInfo;
    } else {
        // Means we already created one before.
        return globalInfo;
    }
}

/**
 * Destructor.
 */
GlobalInfo::~GlobalInfo() {
    instanceFlag = false;
    pthread_mutex_destroy(&lock);
}

/**
 * Add driver to the maps which contain info on this driver.
 * @param driverId id number of the driver.
 * @param socket socket-descriptor of the driver.
 */
void GlobalInfo::addDriverToMap(int driverId, int socket) {
    pthread_mutex_lock(&lock);
    descriptorToDriverId.insert(pair<int, int>(socket, driverId));
    isDriverFinishedCommand.insert(pair<int, bool>(socket, false));
    isNewCommandPerDriver.insert(pair<int, bool>(socket, false));
    pthread_mutex_unlock(&lock);
}

/**
 * Update the current command given us at the main of server.
 * @param mission number.
 * @param clientId optional (if the command is 4)
 */
void GlobalInfo::updateCommand(int mission, int clientId) {
    pthread_mutex_lock(&lock);
    command = mission;
    driverId = clientId;
    map<int, bool>::iterator it;
    for (it = isNewCommandPerDriver.begin();
         it != isNewCommandPerDriver.end(); ++it) {
        it->second = true;
    }
    pthread_mutex_unlock(&lock);
}

/**
 * Get the current command.
 * @return command.
 */
int GlobalInfo::getCurrentCommand() {
    return command;
}

/**
 * Check if after command number 4 was entered,
 * the location of the driver with the given socket descriptor was requested.
 * @param driverSocket socket-descriptor of the driver.
 * @return true if it is requested, false otherwise.
 */
bool GlobalInfo::isDriverLocationRequested(int driverSocket) {
    return descriptorToDriverId.at(driverSocket) == driverId;
}

/**
 * Check if all the drivers finished their last command.
 * @return true if all finished, false otherwise.
 */
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

/**
 * Set all the drivers to the state of "not-finished-command".
 */
void GlobalInfo::setAllDriversToNotFinish() {
    pthread_mutex_lock(&lock);
    map<int, bool>::iterator it;
    for (it = isDriverFinishedCommand.begin();
         it != isDriverFinishedCommand.end(); ++it) {
        it->second = false;
    }
    pthread_mutex_unlock(&lock);
}

/**
 * Set the driver with the given socket-descriptor to "finished-command".
 * @param driverSocket socket-descriptor.
 */
void GlobalInfo::setDriverFinishCommand(int driverSocket) {
    pthread_mutex_lock(&lock);
    isDriverFinishedCommand.at(driverSocket) = true;
    pthread_mutex_unlock(&lock);
}

/**
 * Check if there is a new command.
 * @param driverSocket socket-descriptor.
 * @return true if the driver has new command, false otherwise.
 */
bool GlobalInfo::getIsNewCommand(int driverSocket) {
    return isNewCommandPerDriver.at(driverSocket);
}

/**
 * Set the driver with the given socket-descriptor to "no-new-command" state.
 * @param driverSocket socket-descriptor.
 */
void GlobalInfo::setNotNewCommand(int driverSocket) {
    pthread_mutex_lock(&lock);
    isNewCommandPerDriver.at(driverSocket) = false;
    pthread_mutex_unlock(&lock);
}

/**
 * Check if the driver finished the last command.
 * @param driverSocket socket-descriptor.
 * @return true if he did, false otherwise.
 */
bool GlobalInfo::isDriverFinishCommand(int driverSocket) {
    return isDriverFinishedCommand.at(driverSocket);
}

/**
 * Get the number of clients.
 * @return number.
 */
unsigned long GlobalInfo::getNumClients() {
    return descriptorToDriverId.size();
}
