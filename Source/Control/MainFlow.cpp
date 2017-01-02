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
 * Get location of a driver with the diven id
 * @param id id-number of a certain driver.
 * @return point, which is the location of the driver.
 */
Point MainFlow::requestDriverLocation(int id) {
    return taxiCenter->askDriverLocation(id);
}

/**
 * Add number of drivers to the taxi center.
 * @param numDrivers number of drivers.
 * @param port port number.
 */
void MainFlow::addDrivers(int numDrivers, uint16_t port) {
    taxiCenter->initializeSocketsList(numDrivers, port);
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
void MainFlow::operateTaxiCenter() {
    taxiCenter->operate();
}
