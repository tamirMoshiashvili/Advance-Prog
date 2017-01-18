#include "GlobalInfo.h"

GlobalInfo *GlobalInfo::globalInfo = NULL;
bool GlobalInfo::instanceFlag = false;

/**
 * Constructor.
 * @return GlobalInfo object.
 */
GlobalInfo::GlobalInfo() {
    flag = false;
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

void GlobalInfo::addDriverToMap(int socket) {
    commandPerDriver.insert(pair<int, int>(socket, -1));
    driverToFlag.insert(pair<int, bool>(socket, true));
}

/**
 * Add driver to the maps which contain info on this driver.
 * @param driverId id number of the driver.
 * @param socket socket-descriptor of the driver.
 */
void GlobalInfo::addToSocketToIdMap(int driverId, int socket) {
    pthread_mutex_lock(&lock);
    descriptorToDriverId.insert(pair<int, int>(socket, driverId));
    pthread_mutex_unlock(&lock);
}

/**
 * Update the current command given us at the main of server.
 * @param mission number.
 * @param clientId optional (if the command is 4)
 */
void GlobalInfo::updateCommand(int mission, int clientId) {
    pthread_mutex_lock(&lock);
    driverId = clientId;
    map<int, int>::iterator it;
    for (it = commandPerDriver.begin();
         it != commandPerDriver.end(); ++it) {
        it->second = mission;
    }
    pthread_mutex_unlock(&lock);
}

/**
 * Get the current command.
 * @return command.
 */
int GlobalInfo::getCurrentCommand(int driverSocket) {
    return commandPerDriver.at(driverSocket);
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
//    pthread_mutex_lock(&lock);
    map<int, int>::iterator it;
    for (it = commandPerDriver.begin();
         it != commandPerDriver.end(); ++it) {
        if (it->second != -1) {
            return false;
        }
    }
//    pthread_mutex_unlock(&lock);
    return true;
}


/**
 * Check if there is a new command.
 * @param driverSocket socket-descriptor.
 * @return true if the driver has new command, false otherwise.
 */
bool GlobalInfo::getIsNewCommand(int driverSocket) {
    return commandPerDriver.at(driverSocket) != -1;
}

/**
 * Set the driver with the given socket-descriptor to "no-new-command" state.
 * @param driverSocket socket-descriptor.
 */
void GlobalInfo::setNotNewCommand(int driverSocket) {
    pthread_mutex_lock(&lock);
    commandPerDriver.at(driverSocket) = -1;
    pthread_mutex_unlock(&lock);
}

/**
 * Get the number of clients.
 * @return number.
 */
unsigned long GlobalInfo::getNumClients() {
    return descriptorToDriverId.size();
}

bool GlobalInfo::isFlagTurnOn() {
    pthread_mutex_lock(&lock);
    map<int, bool>::iterator it;
    for (it = driverToFlag.begin(); it != driverToFlag.end(); ++it) {
        if (it->second) {
            return true;
        }
    }
    pthread_mutex_unlock(&lock);
    return false;
}

void GlobalInfo::turnOnFlag() {
    pthread_mutex_lock(&lock);
    map<int, bool>::iterator it;
    for (it = driverToFlag.begin(); it != driverToFlag.end(); ++it) {
        it->second = true;
    }
    pthread_mutex_unlock(&lock);
}

void GlobalInfo::turnOffFlag(int driverSocket) {
    pthread_mutex_lock(&lock);
    driverToFlag.at(driverSocket) = false;
    pthread_mutex_unlock(&lock);
}

pthread_mutex_t *GlobalInfo::getLocker() {
    return &lock;
}




