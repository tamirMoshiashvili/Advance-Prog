#include <iostream>
#include <boost/log/trivial.hpp>
#include "GlobalInfo.h"

GlobalInfo *GlobalInfo::globalInfo = NULL;
bool GlobalInfo::instanceFlag = false;

/**
 * Constructor.
 * @return GlobalInfo object.
 */
GlobalInfo::GlobalInfo() {
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
    map<int, deque<string> *>::iterator it;
    for (it = rideIdToPath.begin();
         it != rideIdToPath.end(); ++it) {
        delete it->second;
    }
}

/**
 * Add the driver with the given socket-descriptor to the maps.
 * @param socket socket-descriptor.
 */
void GlobalInfo::addDriverToMap(int socket) {
    pthread_mutex_lock(&lock);
    commandPerDriver.insert(pair<int, int>(socket, -1));
    driverToFlag.insert(pair<int, bool>(socket, true));
    pthread_mutex_unlock(&lock);
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
    map<int, int>::iterator it;
    for (it = commandPerDriver.begin();
         it != commandPerDriver.end(); ++it) {
        if (it->second != -1) {
            return false;
        }
    }
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
 * Check if the flag is turn-on.
 * @return true if it is, false otherwise.
 */
bool GlobalInfo::isFlagTurnOn() {
    pthread_mutex_lock(&lock);
    map<int, bool>::iterator it;
    for (it = driverToFlag.begin(); it != driverToFlag.end(); ++it) {
        if (it->second) {
            pthread_mutex_unlock(&lock);
            return true;
        }
    }
    pthread_mutex_unlock(&lock);
    return false;
}

/**
 * Turn off the flag.
 * @param driverSocket socket-descriptor.
 */
void GlobalInfo::turnOffFlag(int driverSocket) {
    pthread_mutex_lock(&lock);
    driverToFlag.at(driverSocket) = false;
    pthread_mutex_unlock(&lock);
}

/**
 * Add the pair of the ride-id with the given path to the map.
 * @param rideId id of ride.
 * @param string_path deque of strings which represents the path of the ride.
 */
void GlobalInfo::addRideToMap(int rideId, deque<string> *string_path) {
    pthread_mutex_lock(&lock);
    rideIdToPath.insert(pair<int, deque<string> *>(rideId, string_path));
    pthread_mutex_unlock(&lock);
}

/**
 * Check if the map of rides contains a key rideId.
 * @param rideId id of ride.
 * @return true if it exists in the map, false otherwise.
 */
bool GlobalInfo::doesRideExist(int rideId) {
    pthread_mutex_lock(&lock);
    bool answer = false;
    map<int, deque<string> *>::iterator it = rideIdToPath.find(rideId);
    if (it != rideIdToPath.end()) {
        answer = true;
    }
    pthread_mutex_unlock(&lock);
    return answer;
}

/**
 * Check if a driver with the given Id exist.
 * @param driverId driver id number.
 * @return true if it in the map, false otherwise.
 */
bool GlobalInfo::doesDriverExist(int driverId) {
    bool answer = false;
    pthread_mutex_lock(&lock);
    for (map<int, int>::iterator it = descriptorToDriverId.begin();
         it!=descriptorToDriverId.end();++it){
        if (it->second == driverId) {
            // Driver has been found.
            answer = true;
            break;
        }
    }
    pthread_mutex_unlock(&lock);
    return answer;
}

/**
 * Get the path of the ride with the given id, and delete it from the map.
 * @param rideId id of ride.
 * @return deque of strings, which is the path of the ride.
 */
deque<string> *GlobalInfo::popPathOf(int rideId) {
    deque<string> *path = rideIdToPath.at(rideId);
    rideIdToPath.erase(rideId);
    return path;
}
