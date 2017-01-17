#include "StandardCab.h"

/**
 * Constructor.
 * @param idNum id number.
 * @param manufacturer1 manufactirer of the cab.
 * @param color1 color of the cab.
 * @param tariffVal the tariff of the cab.
 * @param locationDetector detector of blocks location in a matrix.
 * @return StandardCab object.
 */
StandardCab::StandardCab(int idNum, Manufacturer manufacturer1, Color color1,
                         double tariffVal)
        : Cab(idNum, manufacturer1, color1, tariffVal) {

}

/**
 * Default constructor.
 * @return StandardCab object.
 */
StandardCab::StandardCab() : Cab(0, HONDA, RED, 0) {

}

/**
 * Destructor.
 */
StandardCab::~StandardCab() {

}

/**
 * Get the amount of kilometers the cab can go through, in one step.
 * @return number.
 */
int StandardCab::getSizeOfStep() {
    return 1;
}
