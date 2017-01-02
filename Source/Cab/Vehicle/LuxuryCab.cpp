#include "LuxuryCab.h"

/**
 * Constructor.
 * @param idNum id number.
 * @param manufacturer1 manufacturer of the car.
 * @param color1 color of the car.
 * @param tariffVal the tariff of the cab.
 * @param locationDetector detector of blocks location in a matrix.
 * @return LuxuryCab object.
 */
LuxuryCab::LuxuryCab(int idNum, Manufacturer manufacturer1, Color color1,
                     double tariffVal, LocationDetector *locationDetector)
        : Cab(idNum, manufacturer1, color1, tariffVal, locationDetector) {

}

/**
 * Default constructor.
 * @return LuxuryCab object.
 */
LuxuryCab::LuxuryCab() : Cab(0, HONDA, RED, 0, NULL) {

}

/**
 * Destructor.
 */
LuxuryCab::~LuxuryCab() {

}

/**
 * Get the amount of kilometers the cab can go through, in one step.
 * @return number.
 */
int LuxuryCab::getSizeOfStep() {
    return 2;
}
