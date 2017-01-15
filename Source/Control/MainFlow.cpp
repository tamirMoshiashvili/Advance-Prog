#include <iostream>
#include "MainFlow.h"

using namespace std;

/**
 * Constructor.
 * @param center pointer to taxi center.
 * @return MainFlow object.
 */
MainFlow::MainFlow(TaxiCenter *center) : taxiCenter(center) {

}

/**
 * Destructor.
 */
MainFlow::~MainFlow() {
    delete taxiCenter;
}

/**
 * Add number of drivers to the taxi center.
 * @param numDrivers number of drivers.
 * @param port port number.
 */
void MainFlow::addDrivers(int numDrivers, uint16_t port,
                          GlobalInfo *globalInfo) {
    taxiCenter->initialize(numDrivers, port, globalInfo);
}

/**
 * Add a cab to the taxi center.
 * @param cab pointer to cab.
 */
void MainFlow::addCab(Cab *cab) {
    taxiCenter->addCab(cab);
}

/**
 * Add a ride to the taxi center.
 * @param ride pointer to ride.
 */
void MainFlow::addRide(Ride *ride) {
    taxiCenter->addRide(ride);
}

/**
 * Make taxi-center work, operate the drivers and the cabs to handle rides.
 */
void MainFlow::advanceClock() {
    taxiCenter->advanceClock();
}
