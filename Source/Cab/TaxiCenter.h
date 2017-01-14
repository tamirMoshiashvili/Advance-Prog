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
#include "../GlobalInfo.h"
#include <mutex>

/**
 * Represents a taxi center that control the drivers and the cabs.
 */
class TaxiCenter {
private:
    int numOfDrivers;
    TcpServer *tcpServer;
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

    void identifyDriver(int driverSocket, GlobalInfo *globalInfo);

    void sendRide(int driverSocket, Ride *ride);

    Point askDriverLocation(int driverSocket);

    void initialize(int numDrivers, uint16_t port, GlobalInfo *globalInfo);

    void advanceClock();

    void makeDriverWork(int driverSocket, pthread_mutex_t *map_iteration_lock);

private:
    Navigation *produceNavigation(Ride *ride, Point srcDriverPoint);

    void sendNavigation(int driverSocket, Ride *ride);


};


#endif //EX2_TAXICENTER_H

