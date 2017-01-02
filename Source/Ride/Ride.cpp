#include "Ride.h"

/**
 * Constructor.
 * @param idNum id number of the trip.
 * @param start start point.
 * @param end end point.
 * @param numOfPassengers number of passengers in the current trip.
 * @param tariffVal the tariff of the trip.
 * @return Ride object.
 */
Ride::Ride(int idNum, Point start, Point end,
           int numOfPassengers, double tariffVal, int time)
        : Recognizable(idNum), startPoint(start), endPoint(end),
          numPassengers(numOfPassengers), tariff(tariffVal), startTime(time) {
    passengers = list<Passenger *>();
    // Create list of passengers according to the given data.
    for (int i = 0; i < numPassengers; ++i) {
        Passenger *passenger = new Passenger(start, end);
        passengers.push_back(passenger);
    }
}

/**
 * Default constructor.
 * @return Ride object.
 */
Ride::Ride() : Recognizable(0), startPoint(Point(0, 0)), endPoint(Point(0, 0)),
               numPassengers(0), tariff(0) {

}

/**
 * Destructor.
 */
Ride::~Ride() {
    // Delete all the passengers.
    for (std::list<Passenger *>::iterator it = passengers.begin();
         it != passengers.end(); it++) {
        delete (*it);
    }
}

/**
 * Get the tariff of the trip.
 * @return number.
 */
double Ride::getTariff() {
    return tariff;
}

/**
 * Get the list of passengers of the current ride.
 * @return list of pointers to passengers.
 */
list<Passenger *> Ride::getPassengers() {
    return passengers;
}

/**
 * Get the source point of the passenger.
 * @return point.
 */
Point Ride::getSourcePoint() {
    return startPoint;
}

/**
 * Get the destination point of the passenger.
 * @return point.
 */
Point Ride::getDestinationPoint() {
    return endPoint;
}

/**
 * Get the time of the begining of the ride.
 * @return int representing the time.
 */
int Ride::getStartTime() {
    return startTime;
}
