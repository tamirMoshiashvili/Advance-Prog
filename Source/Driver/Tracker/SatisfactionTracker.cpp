#include "SatisfactionTracker.h"

/**
 * Constructor.
 * @param driver1 the driver to track.
 * @param ride the driver's trip.
 * @return SatisfactionTracker object.
 */
SatisfactionTracker::SatisfactionTracker(Driver *driver1, Ride *ride)
        : MovementListener(driver1, ride) {
    // Add all the passengers in the given ride to the driver.
    driver1->addPassengers(ride->getPassengers());
}

/**
 * Default constructor.
 * @return SatisfactionTracker object.
 */
SatisfactionTracker::SatisfactionTracker() : MovementListener() {

}

/**
 * Destructor.
 */
SatisfactionTracker::~SatisfactionTracker() {

}

/**
 * The driver moved, checks if the driver ended his ride,
 * if so, all the passengers need to inspect the driver.
 * @param step number of kilometers passed.
 */
void SatisfactionTracker::moveEvent(int step) {
    Driver *driver = getDriver();
    std::list<Passenger *> passengers = driver->getPassengers();
    if (driver->getCab()->isArrivedToDestination()) {
        if (driver->getLocation() == getRide()->getDestinationPoint()) {
            // Driver ended his ride.
            for (std::list<Passenger *>::iterator it = passengers.begin();
                 it != passengers.end(); it++) {
                // Iterate over the passengers and inspect the driver.
                driver->addInspection((*it)->getSatisfaction());
            }
        }
    }
}
