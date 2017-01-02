#ifndef EX2_MOVEMENTLISTENER_H
#define EX2_MOVEMENTLISTENER_H

#include "../Ride/Ride.h"

class Driver;

/**
 * Represents an object that listen to driver's movement in a certain ride.
 */
class MovementListener {
private:
    Driver *driver;
    Ride *ride;

public:
    MovementListener(Driver *driver1, Ride *trip);

    MovementListener();

    virtual ~MovementListener();

    virtual void moveEvent(int step)=0;

    Ride *getRide();

protected:
    Driver *getDriver();

};


#endif //EX2_MOVEMENTLISTENER_H
