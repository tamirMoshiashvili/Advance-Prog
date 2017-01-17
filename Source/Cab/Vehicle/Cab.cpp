#include "Cab.h"

/**
 * Constructor.
 * @param idNum id number.
 * @param manufacturer1 manufacturer of the cab.
 * @param color1 color of the car.
 * @param tariffVal the tariff of the cab.
 * @param locationDetector detector of blocks location in a matrix.
 * @return Cab object.
 */
Cab::Cab(int idNum, Manufacturer manufacturer1,
         Color color1, double tariffVal, LocationDetector *locationDetector)
        : Recognizable(idNum), manufacturer(manufacturer1),
          color(color1), tariff(tariffVal),
          location(Point(0, 0)), detector(locationDetector),navigation(NULL){
}

/**
 * Dafualt constructor.
 * @return Cab object.
 */
Cab::Cab() : Recognizable(0), manufacturer(HONDA), color(RED), tariff(0),
             location(Point(0, 0)), detector(NULL), navigation(NULL) {

}

/**
 * Destructor.
 */
Cab::~Cab() {

}

/**
 * Get the location of the cab.
 * @return point.
 */
Point Cab::getLocation() {
    return location;
}

/**
 * Set the location of the cab to the given point.
 * @param point new location of the cab.
 */
void Cab::setLocation(Point point) {
    location = point;
}

/**
 * Get the color of the cab.
 * @return color color of the cab.
 */
Color Cab::getColor() {
    return color;
}

/**
 * Set the color of the cab to the given color.
 * @param color1 new color.
 */
void Cab::setColor(Color color1) {
    color = color1;
}

/**
 * Get the tariff of the cab.
 * @return number.
 */
double Cab::getTariff() {
    return tariff;
}

/**
 * Get the manufacturer of the cab.
 * @return cab's manufacturer.
 */
Manufacturer Cab::getManufacturer() {
    return manufacturer;
}

/**
 * Set the navigation of the cab to the given one.
 * @param navigation1 pointer to navigation object.
 */
void Cab::setNavigation(Navigation *navigation1) {
    navigation = navigation1;
}

/**
 * Move the cab to the next location.
 * @return number of kilometers passed in this step.
 */
int Cab::moveOneStep() {
    if (!navigation->hasNextLocation()) {
        // Cab is already at destination point so no movement occurs,
        // means this ride is over so reset the navigation.
        return 0;
    } else {
        // Cab need to reach the destination location.
        int step = getSizeOfStep();
        Block *destBlock = NULL;
        int i;
        // Get the next location of the cab.
        for (i = 0; i < step; ++i) {
            if (!navigation->hasNextLocation()) {
                // Path ended, there are no more locations to be in.
                break;
            }
            destBlock = navigation->getNextLocation();
        }
        // Find its coordinates on the matrix.
        Point newLocation = detector->getLocation(destBlock);
        setLocation(newLocation);
        //delete destBlock;
        // Return the number of kilometers passed.
        return i;
    }
}

/**
 * Ride has ended, reset the old navigation.
 */
void Cab::resetNavigation() {
    if (navigation != NULL) {
        delete navigation;
        navigation = NULL;
    }
}

/**
 * Check if the cab arrived to its destination.
 * @return true if it is, false otherwise.
 */
bool Cab::isArrivedToDestination() {
    return navigation == NULL || !navigation->hasNextLocation();
}

/**
 * Get the location detector of the cab.
 * @return pointer to location detector.
 */
LocationDetector *Cab::getLocationDetector() {
    return detector;
}
