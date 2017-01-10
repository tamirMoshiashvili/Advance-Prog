#ifndef EX2_TAXICENTER_H
#define EX2_TAXICENTER_H

#include <map>
#include "../Driver/Driver.h"
#include "../Ride/Passenger.h"
#include "../Socket/Socket.h"
#include "../Socket/Tcp/TcpServer.h"

#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <boost/archive/binary_iarchive.hpp>

/**
 * Represents a taxi center that control the drivers and the cabs.
 */
class TaxiCenter {
private:
    vector<pthread_t*> threads;
    TcpServer * tcpServer;
    map<int, int> driverIdToDescriptor;
    map<int, Cab *> idToCab;
    map<int, Ride *> idToRides;
    CityMap *cityMap;
    list<Ride *> rides;
    LocationDetector *detector;
    int clock;

public:
    TaxiCenter(CityMap *map);

    ~TaxiCenter();

    void addRide(Ride *tripInfo);

    void removeRide(Ride *tripInfo);

    void removeDriver(int driverID);

    void addCab(Cab *cab);

    void removeCab(Cab *cab);

    int getNumDrivers();

    int getNumCabs();

    int getNumRides();

    LocationDetector *getLocationDetector();

    void sendRide(int driverId, Ride *ride);

    Point askDriverLocation(int driverId);

    void initializeSocketsList(int numDrivers, uint16_t port);

    void operate();

private:
    Navigation *produceNavigation(Ride *ride, Point srcDriverPoint);

    void addDrivers();

    int findAvailableDriver();

    void sendNavigation(int driverId, Ride *ride);

    void makeDriversWork();

    void assignRidesToDrivers();

    void advanceClock();
};


#endif //EX2_TAXICENTER_H

