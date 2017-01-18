#ifndef EX2_MOVEMENTNOTIFIER_H
#define EX2_MOVEMENTNOTIFIER_H


#include "MovementListener.h"
#include <list>
#include <vector>

/**
 * Represents an object that notify others about its movement.
 */
class MovementNotifier {
private:
    std::vector<MovementListener *> listeners;

public:
    MovementNotifier();

    virtual ~MovementNotifier();

    void addListener(MovementListener *listener);

    void clearListeners();

protected:
    void notifyMovement(int step);

    bool exist(std::vector<Ride*> rides, Ride *ride);
};


#endif //EX2_MOVEMENTNOTIFIER_H
