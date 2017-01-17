#include "Driver.h"
#include "Tracker/KilometersPassedTracker.h"
#include "Tracker/SatisfactionTracker.h"

/**
 * Constructor.
 * @param idNum id number.
 * @param ageVal age of the driver.
 * @param status marital status.
 * @param years years of experience.
 * @param cabID id number of the driver's cab.
 * @return Driver object.
 */
Driver::Driver(int idNum, int ageVal, MaritalStatus status,
               int years, int cabID)

        : Recognizable(idNum), age(ageVal), maritalStatus(status),
          yearsOfExp(years), avgSatisfaction(0), totalPassengers(0),
          cab(NULL), cabId(cabID), available(true), location(Point(0, 0)) {

    passengers = list<Passenger *>();
}

/**
 * Default constructor.
 * @return Driver object.
 */
Driver::Driver() : Recognizable(0), age(0), maritalStatus(SINGLE),
                   yearsOfExp(0), avgSatisfaction(0), totalPassengers(0),
                   cab(NULL), cabId(0), available(true), location(Point(0, 0)) {
    passengers = list<Passenger *>();
}

/**
 * Destructor.
 */
Driver::~Driver() {
    delete cab->getLocationDetector();
    delete cab;
}

/**
 * Get the id of the driver's cab.
 * @return number.
 */
int Driver::getCabId() {
    return cabId;
}

/**
 * Get the driver's cab.
 * @return pointer to cab.
 */
Cab *Driver::getCab() {
    return cab;
}

/**
 * Set the cab of the driver.
 * @param cab pointer to some cab.
 */
void Driver::setCab(Cab *taxi) {
    cab = taxi;
}

/**
 * Get the location of the driver.
 * @return point.
 */
Point Driver::getLocation() {
    return location;
}

/**
 * Set the location of the driver.
 * @param point.
 */
void Driver::setLocation(Point point) {
    location = point;
    cab->setLocation(point);
}

/**
 * Notify the driver of a certain ride he must take.
 * @param navigation pointer to navigation, will contain the ride details.
 */
void Driver::takeRide(Navigation *navigation) {
    available = false;
    cab->setNavigation(navigation);
}

/**
 * Check if the driver is available, and has no ride.
 * @return true if he is free, false otherwise.
 */
bool Driver::isAvailable() {
    return available;
}

/**
 * Set the state of the driver to "available".
 * @param state true for available, false for not available.
 */
void Driver::setAvailability(bool state) {
    available = state;
}

/**
 * Drive the cab to its next location in the ride,
 * and notify about the movement.
 */
void Driver::moveOneStep() {
    // Drive the cab.
    int steps = cab->moveOneStep();
    location = cab->getLocation();
    notifyMovement(steps);
    if (cab->isArrivedToDestination()) {
        // This is the end of the ride, Handle listeners and passengers.
        clearListeners();
        passengers.clear();
        // Make driver available to another work.
        available = true;
        cab->resetNavigation();
    }
}

/**
 * Add a list of passengers to the driver.
 * @param passengersList list of pointers to passengers.
 */
void Driver::addPassengers(std::list<Passenger *> passengersList) {
    for (std::list<Passenger *>::iterator it = passengersList.begin();
         it != passengersList.end(); it++) {
        passengers.push_back(*it);
    }
}

/**
 * Get the passengers of the driver.
 * @return list of pointers to passengers.
 */
list<Passenger *> Driver::getPassengers() {
    return passengers;
}

/**
 * Add the given inspection to the average satisfaction.
 * @param satisfaction number between 1 to 5.
 */
void Driver::addInspection(int satisfaction) {
    int numinator = avgSatisfaction * totalPassengers + satisfaction;
    avgSatisfaction = numinator / (++totalPassengers);
}

/**
 * Get the average satsfaction of the driver.
 * @return average satisfaction.
 */
int Driver::getAvgSatisfaction() {
    return avgSatisfaction;
}

/**
 * Add a listeners which associated with the current driver and the given ride.
 * @param ride pointer to ride.
 */
void Driver::addListeners(Ride *ride) {
    MovementListener *wayTracker =
            new KilometersPassedTracker(this, ride);
    addListener(wayTracker);
    MovementListener *satisfactionTracker =
            new SatisfactionTracker(this, ride);
    addListener(satisfactionTracker);
}
