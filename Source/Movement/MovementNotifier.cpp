#include "MovementNotifier.h"
#include "../Driver/Tracker/KilometersPassedTracker.h"

/**
 * Constructor.
 * @return MovementNotifier object.
 */
MovementNotifier::MovementNotifier() {
    listeners = vector<MovementListener *>();
}

/**
 * Destructor.
 */
MovementNotifier::~MovementNotifier() {
    for (unsigned int i = 0; i < listeners.size(); ++i) {
        delete listeners[i];
    }
}

/**
 * Add a listener to this object.
 * @param listener pointer to movement listener.
 */
void MovementNotifier::addListener(MovementListener *listener) {
    listeners.push_back(listener);
}

/**
 * Remove a listener from this object.
 * @param listener pointer to movement listener.
 */
void MovementNotifier::removeListener(MovementListener *listener) {
    for (unsigned int i = 0; i < listeners.size(); ++i) {
        if (listeners[i] == listener) {
            MovementListener *temp = listeners[i];
            listeners.erase(listeners.begin() + i);
            delete temp;
            break;
        }
    }
}

/**
 * Delete all the listeners from the old ride.
 */
void MovementNotifier::clearListeners() {
    std::vector<Ride *> temp = std::vector<Ride *>();
    Ride *old = NULL;
    // Put all the rides in the vector 'temp'.
    for (unsigned int i = 0; i < listeners.size(); i++) {
        // Put the ride only if it is not exits already in the vector.
        old = listeners[i]->getRide();
        if (!exist(temp, old)) {
            temp.push_back(old);
        }
    }
    // Delete all the rides.
    for (unsigned int j = 0; j < temp.size(); ++j) {
        delete temp[j];
    }
    // Delete all the listeners.
    for (unsigned int i = 0; i < listeners.size(); ++i) {
        delete listeners[i];
    }
    // Initialize the listeners.
    listeners = std::vector<MovementListener *>();
}

/**
 * Check if the given ride exist in the vector.
 * Note: comparison is based on the id of the ride.
 * @param rides vector of rides.
 * @param ride pointer to ride object.
 * @return true if the ride exist in the vector, false otherwise.
 */
bool MovementNotifier::exist(std::vector<Ride *> rides, Ride *ride) {
    int rideId = ride->getId();
    for (unsigned int i = 0; i < rides.size(); ++i) {
        // Compare the rides by their ids.
        if (rides[i]->getId() == rideId) {
            // Ride exists.
            return true;
        }
    }
    // Ride does not exist.
    return false;
}

/**
 * Get the list of the listeners of this object.
 * @return list of pointers to movement listeners.
 */
std::vector<MovementListener *> MovementNotifier::getListeners() const {
    return listeners;
}

/**
 * Notify all the listeners about a movement.
 * @param step number of kilometers passed.
 */
void MovementNotifier::notifyMovement(int step) {
    for (unsigned int i = 0; i < listeners.size(); ++i) {
        listeners[i]->moveEvent(step);
    }
}
