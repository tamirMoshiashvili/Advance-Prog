#include "WayPasser.h"

/**
 * Constructor.
 * @return WayPasser object.
 */
WayPasser::WayPasser() {
    numKilometersPassed = 0;
}

/**
 * Destructor.
 */
WayPasser::~WayPasser() {

}

/**
 * Add a value to the way we passed till now.
 * @param num number to increase the value of kilometers passed.
 */
void WayPasser::addKilometersPassed(int num) {
    numKilometersPassed += num;
}

/**
 * Get the number of kilometers passed till now.
 * @return number.
 */
int WayPasser::getNumKilometersPassed() {
    return numKilometersPassed;
}
