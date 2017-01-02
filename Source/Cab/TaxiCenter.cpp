#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "TaxiCenter.h"
#include "../Ride/Navigation/BFS.h"
#include "../Socket/Udp/UdpServer.h"


using namespace std;
using namespace boost;

/**
 * Constructor.
 * @param map pointer to city map.
 * @return TaxiCenter object.
 */
TaxiCenter::TaxiCenter(CityMap *map) : cityMap(map), clock(0) {
    detector = new LocationDetector(cityMap);
}

/**
 * Destructor.
 */
TaxiCenter::~TaxiCenter() {
    //delete cityMap;
    delete detector;
    // Delete drivers.
    closeAllSockets();
    // Delete cabs.
    for (map<int, Cab *>::iterator it = idToCab.begin();
         it != idToCab.end(); ++it) {
        it->second->resetNavigation();
        delete it->second;
    }
    // Delete rides.
    for (map<int, Ride *>::iterator it = idToRides.begin();
         it != idToRides.end(); ++it) {
        delete it->second;
    }
}

/**
 * Close all the sockets from the clients and the socket of the server.
 */
void TaxiCenter::closeAllSockets() {
    // Iterate over the map and send everyone the message.
    for (map<int, Socket *>::iterator it = driverIdToSocket.begin();
         it != driverIdToSocket.end(); ++it) {
        it->second->sendData(END);
        delete it->second;
    }
}

/**
 * Create number of sockets, according to the number of drivers,
 *  with the given port, and initialize them.
 * @param numDrivers number of drivers.
 * @param port port number.
 */
void TaxiCenter::initializeSocketsList(int numDrivers, uint16_t port) {
    // Create socket for each driver.
    for (int i = 0; i < numDrivers; ++i) {
        Socket *sock = new UdpServer((uint16_t) (port + i));
        sock->initialize();
        sockets.push_back(sock);
    }
    // Connect drivers to their sockets.
    addDrivers();
}

/**
 * Add a driver to the taxi center.
 */
void TaxiCenter::addDrivers() {
    char buffer[128];
    int driverId = 0, cabId = 0;
    // Connect each socket to a driver.
    for (list<Socket *>::iterator it = sockets.begin();
         it != sockets.end(); ++it) {
        // Wait for data (Ids) from a driver.
        (*it)->receiveData(buffer, sizeof(buffer));
        // Parse the id of driver and its cab.
        string str(buffer);
        unsigned long i = str.find(",");
        driverId = atoi(str.substr(0, i).c_str());
        cabId = atoi(str.substr(i + 1, str.length()).c_str());
        // Add a driver and its socket to the map.
        driverIdToSocket.insert(pair<int, Socket *>(driverId, *it));
        // Serialize the cab of the driver.
        string serial_str;
        iostreams::back_insert_device<string> inserter(serial_str);
        iostreams::stream<iostreams::back_insert_device<string> > s2(inserter);
        archive::binary_oarchive oa(s2);
        oa << idToCab.at(cabId);
        s2.flush();
        // Send the cab of the driver.
        (*it)->sendData(serial_str);
    }
}

/**
 * Remove a driver from the taxi center.
 * @param int driver id.
 */
void TaxiCenter::removeDriver(int driverID) {

}

/**
 * Add a cab to the taxi center.
 * @param cab pointer to cab.
 */
void TaxiCenter::addCab(Cab *cab) {
    idToCab.insert(std::pair<int, Cab *>(cab->getId(), cab));
}

/**
 * Remove a cab from the taxi center.
 * @param cab pointer to cab.
 */
void TaxiCenter::removeCab(Cab *cab) {
    Cab *temp = idToCab.at(cab->getId());
    idToCab.erase(cab->getId());
    delete temp;
}

/**
 * Add a ride to the taxi center.
 * @param Ride pointer to ride.
 */
void TaxiCenter::addRide(Ride *ride) {
    idToRides.insert(std::pair<int, Ride *>(ride->getId(), ride));
    rides.push_back(ride);
}

/**
 * Remove a ride from the taxi center.
 * @param ride pointer to ride.
 */
void TaxiCenter::removeRide(Ride *ride) {
    Ride *temp = idToRides.at(ride->getId());
    idToRides.erase(ride->getId());
    delete temp;
}

/**
 * Get the number of drivers.
 * @return number.
 */
int TaxiCenter::getNumDrivers() {
    return (int) driverIdToSocket.size();
}

/**
 * Get the number of cabs.
 * @return number.
 */
int TaxiCenter::getNumCabs() {
    return (int) idToCab.size();
}

/**
 * Get the number of rides.
 * @return number.
 */
int TaxiCenter::getNumRides() {
    return (int) idToRides.size();
}

/**
 * Get the location detector.
 * @return pointer to location detector.
 */
LocationDetector *TaxiCenter::getLocationDetector() {
    return detector;
}

/**
 * Send the given ride to the driver with the given id.
 * @param driverId id number of the driver.
 * @param ride pointer to ride object.
 */
void TaxiCenter::sendRide(int driverId, Ride *ride) {
    // Serialize the ride.
    string serial_str;
    iostreams::back_insert_device<string> inserter(serial_str);
    iostreams::stream<iostreams::back_insert_device<string> > stream(inserter);
    archive::binary_oarchive oa(stream);
    oa << ride;
    stream.flush();
    // Send the ride to the driver.
    driverIdToSocket.at(driverId)->sendData(serial_str);
    // Send a navigation-system of the given ride to the driver.
    sendNavigation(driverId, ride);
}

/**
 * Send a navigation system of the given ride, to the driver with the given id.
 * @param driverId id number of the driver.
 * @param ride pointer to ride object.
 */
void TaxiCenter::sendNavigation(int driverId, Ride *ride) {
    // Ask the driver for its location.
    Point driverLocation = askDriverLocation(driverId);
    // Create the navigation-system according to the driver's location.
    Navigation *navigation = produceNavigation(ride, driverLocation);
    // Create opposite stack of blocks ids which represents the path.
    deque<int> oppositePath = navigation->getOppositePath();
    // Serialize the navigation-path.
    string serial_str;
    iostreams::back_insert_device<string> inserter(serial_str);
    iostreams::stream<iostreams::back_insert_device<string> > stream(inserter);
    archive::binary_oarchive oa(stream);
    oa << oppositePath;
    stream.flush();
    // Send the navigation-path to the driver.
    driverIdToSocket.at(driverId)->sendData(serial_str);
    delete navigation;
}

/**
 * Ask the driver, with the given id, for his location.
 * @param driverId id number of the driver.
 * @return point which is the location of the driver on the map.
 */
Point TaxiCenter::askDriverLocation(int driverId) {
    // Send the driver a message which acknowledge him that
    // he need to send his location to the server.
    driverIdToSocket.at(driverId)->sendData(SEND_LOCATION);
    Point driverLocation;
    // De-serialize the location.
    char buffer[128];
    driverIdToSocket.at(driverId)->receiveData(buffer, sizeof(buffer));
    iostreams::basic_array_source<char> device(buffer, sizeof(buffer));
    iostreams::stream<iostreams::basic_array_source<char> > stream(device);
    archive::binary_iarchive ia(stream);
    ia >> driverLocation;
    return driverLocation;
}

/**
 * Produce a navigation-system that match the ride's data.
 * @param ride pointer to ride.
 * @param srcDriverPoint start point of driver.
 * @return pointer to navigation.
 */
Navigation *TaxiCenter::produceNavigation(Ride *ride, Point srcDriverPoint) {
    // Find source of the ride.
    Point srcRidePoint = ride->getSourcePoint();
    Block *src =
            cityMap->getBlock(srcDriverPoint.getX(), srcDriverPoint.getY());
    // Create navigation.
    BFS *bfs = new BFS(src);
    // Check for middle stopping-points.
    if (srcRidePoint != srcDriverPoint) {
        Block *middle =
                cityMap->getBlock(srcRidePoint.getX(), srcRidePoint.getY());
        bfs->addStoppingPoint(middle);
    }
    // Find destination.
    Point destPoint = ride->getDestinationPoint();
    Block *dest = cityMap->getBlock(destPoint.getX(), destPoint.getY());
    bfs->addStoppingPoint(dest);
    bfs->applyAlgorithm();
    return bfs;
}

/**
 * Tell drivers to work, handle the rides and advance the clock.
 */
void TaxiCenter::operate() {
    makeDriversWork();
    assignRidesToDrivers();
    advanceClock();
}

/**
 * Make all the drivers get to their next locations in the ride.
 */
void TaxiCenter::makeDriversWork() {
    // Iterate over the drivers.
    map<int, Socket *>::iterator it;
    for (it = driverIdToSocket.begin(); it != driverIdToSocket.end(); it++) {
        // Send the driver a message which acknowledge him to get to work.
        it->second->sendData(GO);
    }
}

/**
 * Assign the rides that need to be taken at the current tome to drivers.
 */
void TaxiCenter::assignRidesToDrivers() {
    std::list<Ride *> temp;
    // Iterate over the rides list.
    for (list<Ride *>::iterator it = rides.begin(); it != rides.end(); ++it) {
        if ((*it)->getStartTime() == clock) {
            // There is a ride we need to take right now,
            // search for available driver.
            int driverId = findAvailableDriver();
            if (driverId != -1) {
                // Available driver has been found, give him the ride.
                sendRide(driverId, *it);
                // Save the handled ride in the temp-list.
                temp.push_back(*it);
            } else {
                // There are no available drivers, quit the iteration.
                break;
            }
        }
    }
    // Remove all the handled rides.
    for (list<Ride *>::iterator it = temp.begin(); it != temp.end(); ++it) {
        rides.remove(*it);
    }
}

/**
 * Advance the clock by one.
 */
void TaxiCenter::advanceClock() {
    ++clock;
}

/**
 * Find available driver.
 * @return id of available driver, (-1) if there is no available driver.
 */
int TaxiCenter::findAvailableDriver() {
    char buffer[16];
    // Iterate over the drivers sockets.
    map<int, Socket *>::iterator it;
    for (it = driverIdToSocket.begin(); it != driverIdToSocket.end(); ++it) {
        // Ask the driver for availability.
        it->second->sendData(IS_AVAILABLE);
        // Get the driver's answer.
        it->second->receiveData(buffer, sizeof(buffer));
        if (!strcmp(buffer, YES)) {
            // Available driver has been found.
            return it->first;
        }
    }
    // There is no available driver.
    return -1;
}
