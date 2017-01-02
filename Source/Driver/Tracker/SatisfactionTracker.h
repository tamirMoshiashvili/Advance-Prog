#ifndef EX2_SATISFACTIONTRACKER_H
#define EX2_SATISFACTIONTRACKER_H


#include "../Driver.h"

/**
 * Track and update the average satisfaction of a certain driver.
 */
class SatisfactionTracker : public MovementListener {

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<MovementListener>(*this);
    }

public:
    SatisfactionTracker(Driver *driver1, Ride *ride);

    SatisfactionTracker();

    ~SatisfactionTracker();

    void moveEvent(int step);
};


#endif //EX2_SATISFACTIONTRACKER_H
