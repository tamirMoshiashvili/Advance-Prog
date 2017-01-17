#ifndef EX2_DRIVER_H
#define EX2_DRIVER_H

#include "../Cab/Vehicle/Cab.h"
#include "../Basic/Recognizable.h"
#include "../Movement/MovementNotifier.h"
#include "../Ride/Passenger.h"
#include "../Cab/TaxiCenter.h"
#include "../Ride/Navigation/Navigation.h"

/**
 * Represents a marital status of a person.
 */
enum MaritalStatus {
    SINGLE, MARRIED, DIVORCED, WIDOWED
};

class MovementNotifier;

/**
 * Represents a driver of a cab.
 */
class Driver : public MovementNotifier, public Recognizable {
private:
    int age;
    MaritalStatus maritalStatus;
    int yearsOfExp;
    int avgSatisfaction;
    int totalPassengers;
    Cab *cab;
    int cabId;
    bool available;
    list<Passenger *> passengers;
    Point location;

public:
    Driver(int idNum, int ageVal, MaritalStatus status, int years, int cabID);

    Driver();

    ~Driver();

    int getCabId();

    Cab *getCab();

    void setCab(Cab *taxi);

    Point getLocation();

    void setLocation(Point point);

    bool isAvailable();

    void setAvailability(bool state);

    void moveOneStep();

    void addPassengers(list<Passenger *> passengersList);

    list<Passenger *> getPassengers();

    void addInspection(int satisfaction);

    int getAvgSatisfaction();

protected:
    void takeRide(Navigation *navigation);

    void addListeners(Ride *ride);
};


#endif //EX2_DRIVER_H
