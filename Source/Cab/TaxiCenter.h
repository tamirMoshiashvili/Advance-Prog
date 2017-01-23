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
#include "../ThreadControl/GlobalInfo.h"
#include "../ThreadPool/ThreadPool.h"


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
    int clock;
    vector<pthread_t *> threads;
    pthread_mutex_t locker;
    ThreadPool ridesThreadPool;
    pthread_mutex_t rides_lock;

public:
    TaxiCenter(CityMap *map);

    ~TaxiCenter();

    void addRide(Ride *tripInfo);

    void addCab(Cab *cab);

    void identifyDriver(int driverSocket);

    void sendRide(int driverSocket, Ride *ride);

    Point askDriverLocation(int driverSocket);

    void initialize(int numDrivers, uint16_t port);

    void advanceClock();

    void makeDriverWork(int driverSocket);

private:
    void sendNavigation(int driverSocket, Ride *ride);
};


#endif //EX2_TAXICENTER_H


