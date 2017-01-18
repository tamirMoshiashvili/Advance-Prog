#include <cstdlib>
#include "Passenger.h"

/**
 * Constructor.
 * @param source source point.
 * @param destination destination point.
 * @return Passenger object.
 */
Passenger::Passenger(Point source, Point destination)
        : sourcePoint(source), destinationPoint(destination) {

}

/**
 * Default constructor.
 * @return Passenger object.
 */
Passenger::Passenger() : sourcePoint(Point()), destinationPoint(Point()) {

}

/**
 * Destructor.
 */
Passenger::~Passenger() {

}

/**
 * Get the satisfaction of the passenger on some driver.
 * @return random number.
 */
int Passenger::getSatisfaction() {
    return (rand() % 5 + 1);
}
