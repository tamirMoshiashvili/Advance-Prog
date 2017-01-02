#ifndef EX2_TRACKDRIVER_H
#define EX2_TRACKDRIVER_H


#include "../Driver.h"

/**
 * Update the kilometers passed of a ride with a certain driver.
 */
class KilometersPassedTracker : public MovementListener {

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<MovementListener>(*this);
    }

public:
    KilometersPassedTracker(Driver *driver1, Ride *ride);

    KilometersPassedTracker();

    ~KilometersPassedTracker();

    void moveEvent(int step);

};


#endif //EX2_TRACKDRIVER_H
