#include "MovementListener.h"

/**
 * Constructor.
 * @param driver1 the driver to listen to.
 * @param trip the driver's ride.
 * @return MovementListener object.
 */
MovementListener::MovementListener(Driver *driver1, Ride *trip)
        : driver(driver1), ride(trip) {

}

/**
 * Default constructor.
 * @return MovementListener object.
 */
MovementListener::MovementListener() : driver(NULL), ride(NULL) {

}

/**
 * Destructor.
 */
MovementListener::~MovementListener() {

}

/**
 * Get the driver who we listens to.
 * @return pointer to driver.
 */
Driver *MovementListener::getDriver() {
    return driver;
}

/**
 * Get the ride which the drivers takes.
 * @return pointer to Ride.
 */
Ride *MovementListener::getRide() {
    return ride;
}
