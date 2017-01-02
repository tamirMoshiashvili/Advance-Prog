#include "KilometersPassedTracker.h"

/**
 * Constructor.
 * @param driver1 the driver to track.
 * @param ride the driver's trip.
 * @return KilometersPassedTracker object.
 */
KilometersPassedTracker::KilometersPassedTracker(Driver *driver1, Ride *ride) :
        MovementListener(driver1, ride) {

}

/**
 * Default constructor.
 * @return KilometersPassedTracker object.
 */
KilometersPassedTracker::KilometersPassedTracker() : MovementListener() {

}

/**
 * Destructor.
 */
KilometersPassedTracker::~KilometersPassedTracker() {

}

/**
 * The driver moved so we need to notify the trip the driver takes.
 * @param step number of kilometers passed.
 */
void KilometersPassedTracker::moveEvent(int step) {
    getRide()->addKilometersPassed(step);
}
